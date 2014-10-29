#include "debug.h"
#include <QThread>
#include <QTimer>
#include <QScopedPointer>

#include "nzmqt/nzmqt.hpp"

#include "subscriber.h"
#include "zmqsockets.h"
#include "rbcommands.h"
#include "model/jsbridge.h"
#include "model/appstate.h"


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



nzmqt::ZMQContext *Subscriber::getContext() const
{
    return context;
}

void Subscriber::setContext(nzmqt::ZMQContext *value)
{
    context = value;
}
Subscriber::Subscriber(RBKit::JsBridge* bridge)
    :jsBridge(bridge), connectionEstablished(false)
{
    ENTER0("");
    EXIT0("");
}

void Subscriber::triggerGc()
{
    ENTER0("");

    RBKit::CmdTriggerGC triggerGC_Command;
    qDebug() << "Triggering GC";
    commandSocket->sendCommand(triggerGC_Command);
}

void Subscriber::takeSnapshot()
{
    ENTER0("");

   RBKit::CmdObjSnapshot triggerSnapshot;
   qDebug() << "Taking snapshot";
   commandSocket->sendCommand(triggerSnapshot);
}

void Subscriber::startSubscriber()
{
    qDebug() << "start subscriber";
    ENTER0("");

    context = new nzmqt::SocketNotifierZMQContext(this, 1);
    commandSocket = new RBKit::ZmqCommandSocket(this, context);
    eventSocket   = new RBKit::ZmqEventSocket(this, context);
    objectStore = new RBKit::ObjectStore();
    connect(eventSocket->getSocket(), SIGNAL(messageReceived(const QList<QByteArray>&)),
            this, SLOT(onMessageReceived(const QList<QByteArray>&)));

    // initialize the timer, mark it a periodic one, and connect to timeout.
    m_timer = new QTimer(this);
    m_timer->setInterval(timerIntervalInMs);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(onTimerExpiry()));
}

void Subscriber::handShakeCompleted()
{
    try
    {
        eventSocket->start(eventServerUrl);
    }
    catch(zmq::error_t err)
    {
        emitConnectionError(QString::fromUtf8(err.what()));
        return;
    }
    connectionEstablished = true;

    RBKit::CmdStartProfile startCmd;
    commandSocket->sendCommand(startCmd);

    m_timer->start();

    emit connected();
    qDebug("started 12");
    EXIT0("");
}

void Subscriber::emitConnectionError(QString message)
{
    qDebug() << message;
    emit errored(message);
}

Subscriber::~Subscriber()
{
    ENTER0("");
    stop();
    delete m_timer;
    delete commandSocket;
    delete eventSocket;
    delete objectStore;
    delete context;
    emit disconnected();
}

void Subscriber::startListening(QString _commandsUrl, QString _eventsUrl)
{
    this->commandUrl = _commandsUrl;
    this->eventServerUrl = _eventsUrl;
    performHandshake();
}

void Subscriber::stop()
{
    if (connectionEstablished) {
        RBKit::CmdStopProfile stopCmd;
        commandSocket->sendCommand(stopCmd);
    }
    objectStore->reset();
    qDebug() << "stopped";
}


void Subscriber::onMessageReceived(const QList<QByteArray>& rawMessage)
{
    ENTER0("");

    for (QList<QByteArray>::ConstIterator iter = rawMessage.begin();
         rawMessage.end() != iter; ++iter)
    {
        const RBKit::EventParser eventParser(*iter);
        RBKit::EventDataBase* event = eventParser.parseEvent();

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
}


void Subscriber::processEvent(const RBKit::EvtObjectDump& heapDump)
{
    emit dumpReceived(*(msgpack::unpacked*)(&heapDump.dump));

    // objectStore->updateFromSnapshot(dump.objects);

    // RBKit::AppState::getInstance()->setAppState("heap_snapshot", 10);
    // RBKit::SqlConnectionPool::getInstance()->loadSnapshot(objectStore);
    // emit objectDumpAvailable(RBKit::SqlConnectionPool::getInstance()->getCurrentVersion());
}

void Subscriber::processEvent(const RBKit::EvtCollection& evtCollection)
{
    for (auto& event : evtCollection.events) {
        event->process(*this);
    }
}

void Subscriber::performHandshake()
{
    context->start();
    try
    {
        qDebug() << "Connecting to command socket " << commandUrl;
        commandSocket->start(commandUrl);
        if (commandSocket->performHandShake())
            handShakeCompleted();
        else {
            emitConnectionError(QString("Error connecting to Ruby application"));
        }
    }
    catch(zmq::error_t err)
    {
        emitConnectionError(QString::fromUtf8(err.what()));
    }
}


void Subscriber::onTimerExpiry()
{
    static const QString eventName("object_stats");

    QVariantMap map = hashToQVarMap(objectStore->liveStats());
    jsBridge->sendMapToJs(eventName, QDateTime(), map);
}
