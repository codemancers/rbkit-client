#include "zmqsockets.h"
#include "rbcommands.h"
#include <QDebug>

#include "nzmqt/nzmqt.hpp"


static const int rbkcZmqTotalIoThreads = 1;
static const int timerIntervalInMs = 1000;


RBKit::ZmqCommandSocket::ZmqCommandSocket(QObject* parent) :
    QObject(parent)
{
    // initialize command socket timer, so that response is received.
    timer.reset(new QTimer(this));
    timer->setInterval(timerIntervalInMs);
    timer->setSingleShot(true);
    connect(timer.data(), SIGNAL(timeout()), this, SLOT(onTimerExpiry()));

    recreateContextAndSocket();
}

RBKit::ZmqCommandSocket::~ZmqCommandSocket()
{
    disconnect(socket.data());
    socket->close();
}


void RBKit::ZmqCommandSocket::recreateContextAndSocket()
{
    // close the socket if its already opened, because we are going to recreate
    // the socket.
    if (! socket.isNull()) {
        disconnect(socket.data());
        socket->close();
    }

    // second argument to creating a context is number of io threads. right
    // now, we are using only 1 thread, so defaulting to 1 for now.
    context.reset(new nzmqt::SocketNotifierZMQContext(this, rbkcZmqTotalIoThreads));
    socket.reset(context->createSocket(nzmqt::ZMQSocket::TYP_REQ, this));

    connect(socket.data(), SIGNAL(messageReceived(const QList<QByteArray>&)),
            this, SLOT(onResponseReceived(const QList<QByteArray>&)));
}


void RBKit::ZmqCommandSocket::start(QString socketUrl)
{
    QByteArray ba   = socketUrl.toLocal8Bit();
    const char *url = ba.data();
    socket->connectTo(url);

    context->start();
}

void RBKit::ZmqCommandSocket::stop()
{
    context->stop();
}


bool RBKit::ZmqCommandSocket::sendCommand(QSharedPointer<RBKit::CommandBase> cmd)
{
    if (currentCmd.isNull()) {
        // earlier command was successful.
        qDebug() << "Sending " << cmd->serialize().toLocal8Bit();
        nzmqt::ZMQMessage msg(cmd->serialize().toLocal8Bit());

        timer->start();
        bool sent = socket->sendMessage(msg);
        qDebug() << "command sent: " << sent;

        // store the command sent.
        currentCmd.swap(cmd);
        return sent;
    } else {
        // fatal. we haven't yet received response to previous cmd
        qDebug() << "response not received for previous cmd";
        return false;
    }
}

void RBKit::ZmqCommandSocket::onResponseReceived(const QList<QByteArray>& rawMessage)
{
    timer->stop();

    QString response(rawMessage.first());
    qDebug() << "response received" << response;

    bool success = "ok" == response;
    emit commandSent(currentCmd, success);
    currentCmd.clear();
}


void RBKit::ZmqCommandSocket::onTimerExpiry()
{
    // fatal error
    qDebug() << "fatal error, no response received";
    emit commandSent(currentCmd, false);

    // since we have not received any response, this socket is unusable.
    // so, reset it
    recreateContextAndSocket();
    currentCmd.clear();
}



RBKit::ZmqEventSocket::ZmqEventSocket(QObject* parent) :
    QObject(parent)
{
    // second argument to creating a context is number of io threads. right
    // now, we are using only 1 thread, so defaulting to 1 for now.
    context.reset(new nzmqt::SocketNotifierZMQContext(this, rbkcZmqTotalIoThreads));
    socket.reset(context->createSocket(nzmqt::ZMQSocket::TYP_SUB, this));
    socket->setOption(nzmqt::ZMQSocket::OPT_SUBSCRIBE, "", 0);
    socket->setOption(nzmqt::ZMQSocket::OPT_RCVHWM, 500000);
}

RBKit::ZmqEventSocket::~ZmqEventSocket()
{
    socket->close();
}

void RBKit::ZmqEventSocket::start(QString socketUrl)
{
    // There is something wrong with the way how contexts are shared!
    // calling context->isStopped() returned false

    QByteArray ba   = socketUrl.toLocal8Bit();
    const char *url = ba.data();
    socket->connectTo(url);

    context->start();
}


void RBKit::ZmqEventSocket::stop()
{
    context->stop();
}
