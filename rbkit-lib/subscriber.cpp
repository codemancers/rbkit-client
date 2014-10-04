#include <QDebug>
#include <QThread>
#include <QTimer>
#include <QScopedPointer>

#include "nzmqt/nzmqt.hpp"

#include "subscriber.h"
#include "zmqsockets.h"
#include "rbcommands.h"
#include "model/jsbridge.h"
#include "model/appstate.h"


static const int rbkcZmqTotalIoThreads = 1;
static const int timerIntervalInMs = 1500;

// TODO: Move it to utils, or some common file.
template <class K, class V>
QVariantMap hashToQVarMap(const QHash<K, V>&& hash) {
    QVariantMap map;

    auto iter = hash.constBegin();
    for (; iter != hash.constEnd(); ++iter) {
        map[iter.key()] = iter.value();
    }

    return map;
}


Subscriber::Subscriber(RBKit::JsBridge* bridge)
    :jsBridge(bridge)
{
    commandSocket = new RBKit::ZmqCommandSocket(this);
    eventSocket   = new RBKit::ZmqEventSocket(this);
    connect(commandSocket, SIGNAL(commandSent(QSharedPointer<RBKit::CommandBase>, bool)),
            this, SLOT(onCommandSent(QSharedPointer<RBKit::CommandBase>, bool)));

    connect(eventSocket->getSocket(), SIGNAL(messageReceived(const QList<QByteArray>&)),
           this, SLOT(onMessageReceived(const QList<QByteArray>&)));

    objectStore = new RBKit::ObjectStore();

    // initialize the timer, mark it a periodic one, and connect to timeout.
    statsTimer = new QTimer(this);
    statsTimer->setInterval(timerIntervalInMs);
    connect(statsTimer, SIGNAL(timeout()), this, SLOT(onStatsTimerExpiry()));
}

void Subscriber::triggerGc() {
    QSharedPointer<RBKit::CommandBase> triggerGc(new RBKit::CmdTriggerGC());
    qDebug() << "Triggering GC";
    commandSocket->sendCommand(triggerGc);
}

void Subscriber::takeSnapshot()
{
    QSharedPointer<RBKit::CommandBase> snapshotCmd(new RBKit::CmdObjSnapshot());
   qDebug() << "Taking snapshot";
   commandSocket->sendCommand(snapshotCmd);
}

Subscriber::~Subscriber()
{
    stop();

    commandSocket->stop();
    delete commandSocket;

    eventSocket->stop();
    delete eventSocket;

    emit disconnected();
}

void Subscriber::startListening(QString commandsUrl, QString eventsUrl)
{
    qDebug() << "Got " << commandsUrl << eventsUrl;

    try
    {
        commandSocket->start(commandsUrl);
        eventSocket->start(eventsUrl);
    }
    catch(zmq::error_t err)
    {
        QString str = QString::fromUtf8(err.what());
        qDebug() << str ;
        emit errored(str);
        return;
    }

    QSharedPointer<RBKit::CommandBase> startCommand(new RBKit::CmdStartProfile());
    commandSocket->sendCommand(startCommand);

    statsTimer->start();

    qDebug() << "started";
}

void Subscriber::stop()
{
    QSharedPointer<RBKit::CommandBase> stopCmd(new RBKit::CmdStopProfile());
    commandSocket->sendCommand(stopCmd);
    objectStore->reset();
    qDebug() << "stopped";
}


void Subscriber::onMessageReceived(const QList<QByteArray>& rawMessage)
{
    for (QList<QByteArray>::ConstIterator iter = rawMessage.begin();
         rawMessage.end() != iter; ++iter)
    {
        RBKit::EventDataBase* event = RBKit::parseEvent(*iter);

        if (NULL != event) {
            event->process(*this);
        }

        delete event;
    }
}


void Subscriber::processEvent(const RBKit::EvtNewObject& objCreated)
{
    objectStore->addObject(objCreated.object);
}

void Subscriber::processEvent(const RBKit::EvtDelObject& objDeleted)
{
    quint64 objectId = objDeleted.objectId;
    objectStore->removeObject(objectId);
}

void Subscriber::processEvent(const RBKit::EvtGcStats& stats)
{
    static const QString eventName("gc_stats");
    jsBridge->sendMapToJs(eventName, stats.timestamp, stats.payload);
    objectStore->onGcStats(stats.payload);
}


void Subscriber::processEvent(const RBKit::EvtGcStart &gcEvent) {
    qDebug() << "Received gc start" << gcEvent.timestamp;
    static const QString eventName("gc_start");
    QVariantMap map;
    jsBridge->sendMapToJs(eventName, gcEvent.timestamp, map);
}

void Subscriber::processEvent(const RBKit::EvtGcStop &gcEvent)
{
    qDebug() << "Received gc stop" << gcEvent.timestamp;
    static const QString eventName("gc_stop");
    // update generation of objects that have survived the GC
    objectStore->updateObjectGeneration();
    jsBridge->sendMapToJs(eventName, gcEvent.timestamp, QVariantMap());

    // for now, disable polar charts about generations.
    // QVariantMap youngGen = hashToQVarMap(objectStore->youngGenStats());
    // jsBridge->sendMapToJs("young_gen", gcEvent.timestamp, youngGen);

    // QVariantMap secondGen = hashToQVarMap(objectStore->secondGenStats());
    // jsBridge->sendMapToJs("second_gen", gcEvent.timestamp, secondGen);

    // QVariantMap oldGen = hashToQVarMap(objectStore->oldGenStats());
    // jsBridge->sendMapToJs("old_gen", gcEvent.timestamp, oldGen);
}


void Subscriber::processEvent(const RBKit::EvtObjectDump& dump)
{
    objectStore->updateFromSnapshot(dump.objects);

    RBKit::AppState::getInstance()->setAppState("heap_snapshot", 10);
    RBKit::SqlConnectionPool::getInstance()->loadSnapshot(objectStore);
    emit objectDumpAvailable(RBKit::SqlConnectionPool::getInstance()->getCurrentVersion());
}

void Subscriber::processEvent(const RBKit::EvtCollection& evtCollection)
{
    for (auto& event : evtCollection.events) {
        event->process(*this);
    }
}


void Subscriber::onStatsTimerExpiry()
{
    static const QString eventName("object_stats");

    QVariantMap map = hashToQVarMap(objectStore->liveStats());
    jsBridge->sendMapToJs(eventName, QDateTime(), map);
}


void Subscriber::onCommandSent(QSharedPointer<RBKit::CommandBase> cmd, bool success)
{
    qDebug() << success;

    if (success && dynamic_cast<RBKit::CmdStartProfile*>(cmd.data())) {
        emit connected();
    }
}
