#include "zmqsockets.h"
#include "rbcommands.h"
#include <QDebug>

#include "nzmqt/nzmqt.hpp"


static const int rbkcZmqTotalIoThreads = 1;
static const int timerIntervalInMs = 1000;


RBKit::ZmqCommandSocket::ZmqCommandSocket(QObject* parent) :
    QObject(parent)
{
    // second argument to creating a context is number of io threads. right
    // now, we are using only 1 thread, so defaulting to 1 for now.
    context = new nzmqt::SocketNotifierZMQContext(this, rbkcZmqTotalIoThreads);
    socket = context->createSocket(nzmqt::ZMQSocket::TYP_REQ, this);
}

RBKit::ZmqCommandSocket::~ZmqCommandSocket()
{
    socket->close();
    delete socket;
    delete context;
}

bool RBKit::ZmqCommandSocket::sendCommand(RBKit::CommandBase& cmd)
{
    nzmqt::ZMQMessage msg(cmd.serialize().toLocal8Bit());
    bool sent = socket->sendMessage(msg);
    qDebug() << sent;

    QList<QByteArray> resp = socket->receiveMessage();
    qDebug() << resp;

    return true;
}

void RBKit::ZmqCommandSocket::start(QString socketUrl)
{
    if (context->isStopped()) {
        return;
    }

    QByteArray ba   = socketUrl.toLocal8Bit();
    const char *url = ba.data();
    socket->connectTo(url);

    context->start();
}


void RBKit::ZmqCommandSocket::stop()
{
    context->stop();
}


RBKit::ZmqEventSocket::ZmqEventSocket(QObject* parent) :
    QObject(parent)
{
    // second argument to creating a context is number of io threads. right
    // now, we are using only 1 thread, so defaulting to 1 for now.
    context = new nzmqt::SocketNotifierZMQContext(this, rbkcZmqTotalIoThreads);
    socket = context->createSocket(nzmqt::ZMQSocket::TYP_SUB, this);
    socket->setOption(nzmqt::ZMQSocket::OPT_SUBSCRIBE, "", 0);
}

RBKit::ZmqEventSocket::~ZmqEventSocket()
{
    socket->close();
    delete socket;
    delete context;
}

void RBKit::ZmqEventSocket::start(QString socketUrl)
{
    if (!context->isStopped()) {
        return;
    }

    QByteArray ba   = socketUrl.toLocal8Bit();
    const char *url = ba.data();
    socket->connectTo(url);

    context->start();
}


void RBKit::ZmqEventSocket::stop()
{
    context->stop();
}
