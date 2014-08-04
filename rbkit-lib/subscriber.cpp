#include <QDebug>
#include <QThread>
#include <QTimer>

#include "nzmqt/nzmqt.hpp"

#include "subscriber.h"
#include "zmqsockets.h"
#include "rbcommands.h"


static const int rbkcZmqTotalIoThreads = 1;
static const int timerIntervalInMs = 1000;


Subscriber::Subscriber(QObject *parent) :
    QObject(parent)
{
    commandSocket = new RBKit::ZmqCommandSocket(this);
    eventSocket   = new RBKit::ZmqEventSocket(this);
    objectStore = new ObjectStore();
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
    ObjectDetail *objectDetail = new ObjectDetail(objCreated.className, objCreated.objectId);
    objectStore->addObject(objectDetail);
}

void Subscriber::processEvent(const RBKit::EvtDelObject& objDeleted)
{
    qDebug() << "processing obj destroyed";
    quint64 objectId = objDeleted.objectId;
    objectStore->removeObject(objectId);
}


void Subscriber::onTimerExpiry()
{
    // qDebug() << m_type2Count;
    QVariantMap data = objectStore->getObjectTypeCountMap();
    qDebug() << data;
    emit messageReady(data);
}
