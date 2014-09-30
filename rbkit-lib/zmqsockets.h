#ifndef RBKIT_ZMQ_SOCKETS_H
#define RBKIT_ZMQ_SOCKETS_H

#include <QObject>
#include <QSharedPointer>
#include <QTimer>

// forward declaration of nzmqt classes.
namespace nzmqt
{
   class ZMQContext;
   class ZMQSocket;
}


namespace RBKit
{
    class CommandBase;

    class ZmqCommandSocket : public QObject
    {
        Q_OBJECT

        QSharedPointer<nzmqt::ZMQContext> context;
        QSharedPointer<nzmqt::ZMQSocket> socket;

        QSharedPointer<CommandBase> currentCmd;
        QSharedPointer<QTimer> timer;

    public:
        explicit ZmqCommandSocket(QObject* parent = 0);
        ~ZmqCommandSocket();

    public:
        void start(QString socketUrl);
        void stop();

    signals:
        void commandSent(QSharedPointer<RBKit::CommandBase>, bool);

    public slots:
        void onResponseReceived(const QList<QByteArray>&);
        void onTimerExpiry();

    public:
        bool sendCommand(QSharedPointer<CommandBase> cmd);

    private:
        void recreateContextAndSocket();
    };

    class ZmqEventSocket : public QObject
    {
        Q_OBJECT

        QSharedPointer<nzmqt::ZMQContext> context;
        QSharedPointer<nzmqt::ZMQSocket> socket;

    public:
        explicit ZmqEventSocket(QObject* parent = 0);
        ~ZmqEventSocket();

        // hack, not sure how to bind socket receive message.
        inline nzmqt::ZMQSocket* getSocket() {
            return socket.data();
        }

    public:
        void start(QString socketUrl);
        void stop();
    };
};


#endif // RBKIT_ZMQ_SOCKETS_H
