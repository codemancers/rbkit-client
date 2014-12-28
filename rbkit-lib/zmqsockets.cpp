#include "zmqsockets.h"
#include "rbcommands.h"
#include <QDebug>

#include "nzmqt/nzmqt.hpp"
#include <QThread>
#include "rbeventparser.h"
#include "model/appstate.h"


static const int rbkcZmqTotalIoThreads = 1;
static const int timerIntervalInMs = 1000;


RBKit::ZmqCommandSocket::ZmqCommandSocket(QObject* parent, nzmqt::ZMQContext *_context) :
    QObject(parent), context(_context)
{
    // second argument to creating a context is number of io threads. right
    // now, we are using only 1 thread, so defaulting to 1 for now.
    socket = context->createSocket(nzmqt::ZMQSocket::TYP_REQ, this);
    socket->setOption(nzmqt::ZMQSocket::OPT_LINGER, 0);
}

RBKit::ZmqCommandSocket::~ZmqCommandSocket()
{
    qDebug() << "++ thread id is : " << QThread::currentThreadId();
    socket->close();
    delete socket;
}

bool RBKit::ZmqCommandSocket::sendCommand(RBKit::CommandBase& cmd)
{
    try {
        qDebug() << "Sending " << cmd.serialize().toLocal8Bit();
        nzmqt::ZMQMessage msg(cmd.serialize().toLocal8Bit());
        bool sent = socket->sendMessage(msg);

        qDebug() << "Waiting for response";
        QList<QByteArray> resp = socket->receiveMessage();
        qDebug() << "Got a response " << resp;

        return true;
    } catch(zmq::error_t err) {
        QString str = QString::fromUtf8(err.what());
        qDebug() << str ;
        return false;
    }
}

bool RBKit::ZmqCommandSocket::performHandShake()
{
    RBKit::CmdHandshake handShake;
    nzmqt::ZMQMessage msg(handShake.serialize().toLocal8Bit());
    bool sent = socket->sendMessage(msg);
    if(sent) {
        QByteArray response = socket->receiveBlockingMessage();
        EventParser parser(response);
        EvtHandshake *handShake = parser.parseHandShake();
        RBKit::AppState::getInstance()->setAppState("process_name", handShake->processName);
        RBKit::AppState::getInstance()->setAppState("pwd", handShake->pwd);
        RBKit::AppState::getInstance()->setAppState("pid", handShake->pid);
        if (handShake != NULL) {
            return true;
            delete handShake;
        } else {
            return false;
        }
    } else {
        qDebug() << "Failed to perform handshake";
        return false;
    }
}

void RBKit::ZmqCommandSocket::start(QString socketUrl)
{
    QByteArray ba   = socketUrl.toLocal8Bit();
    const char *url = ba.data();
    socket->connectTo(url);
}

void RBKit::ZmqCommandSocket::stop()
{
}


RBKit::ZmqEventSocket::ZmqEventSocket(QObject* parent, nzmqt::ZMQContext *_context) :
    QObject(parent), context(_context)
{
    // second argument to creating a context is number of io threads. right
    // now, we are using only 1 thread, so defaulting to 1 for now.
    socket = context->createSocket(nzmqt::ZMQSocket::TYP_SUB, this);
    socket->setOption(nzmqt::ZMQSocket::OPT_SUBSCRIBE, "", 0);
    socket->setOption(nzmqt::ZMQSocket::OPT_RCVHWM, 500000);
}

RBKit::ZmqEventSocket::~ZmqEventSocket()
{
    socket->close();
    delete socket;
}

void RBKit::ZmqEventSocket::start(QString socketUrl)
{
    // There is something wrong with the way how contexts are shared!
    // calling context->isStopped() returned false

    QByteArray ba   = socketUrl.toLocal8Bit();
    const char *url = ba.data();
    socket->connectTo(url);
}


void RBKit::ZmqEventSocket::stop()
{
}
