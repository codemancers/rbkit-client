#include <QDebug>
#include <QThread>
#include <QTimer>

#include "nzmqt/nzmqt.hpp"

#include "subscriber.h"
#include "zmqsockets.h"
#include "rbcommands.h"
#include "jsbridge.h"


static const int rbkcZmqTotalIoThreads = 1;
static const int timerIntervalInMs = 1000;


Subscriber::Subscriber(QObject *parent, RBKit::JsBridge* bridge)
    : QObject(parent)
    , jsBridge(bridge)
{
    commandSocket = new RBKit::ZmqCommandSocket(this);
    eventSocket   = new RBKit::ZmqEventSocket(this);
    objectStore = new RBKit::ObjectStore();
    connect(eventSocket->getSocket(), SIGNAL(messageReceived(const QList<QByteArray>&)),
           this, SLOT(onMessageReceived(const QList<QByteArray>&)));

    // initialize the timer, mark it a periodic one, and connect to timeout.
    m_timer = new QTimer(this);
    m_timer->setInterval(timerIntervalInMs);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(onTimerExpiry()));
}

void Subscriber::triggerGc() {
    RBKit::CmdTriggerGC triggerGC_Command;
    qDebug() << "Triggering GC";
    commandSocket->sendCommand(triggerGC_Command);
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

    RBKit::CmdStartProfile startCmd;
    commandSocket->sendCommand(startCmd);

    m_timer->start();

    emit connected();
    qDebug() << "started";
}

void Subscriber::stop()
{
    RBKit::CmdStopProfile stopCmd;
    commandSocket->sendCommand(stopCmd);

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
    RBKit::ObjectDetail *objectDetail = new RBKit::ObjectDetail(objCreated.className, objCreated.objectId);
    objectStore->addObject(objectDetail);
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
    QVariantMap map;
    jsBridge->sendMapToJs(eventName, gcEvent.timestamp, map);
}

void Subscriber::onTimerExpiry()
{
    static const QString eventName("object_stats");

    // qDebug() << m_type2Count;
    QVariantMap data = objectStore->getObjectTypeCountMap();
    jsBridge->sendMapToJs(eventName, QDateTime(), data);
}
