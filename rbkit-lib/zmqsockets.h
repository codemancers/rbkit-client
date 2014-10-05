#ifndef RBKIT_ZMQ_SOCKETS_H
#define RBKIT_ZMQ_SOCKETS_H

#include <QObject>
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

        nzmqt::ZMQContext* context;
        nzmqt::ZMQSocket* socket;
        QTimer *handShakeTimer;

    public:
        explicit ZmqCommandSocket(QObject* parent = 0);
        ~ZmqCommandSocket();

    public:
        void start(QString socketUrl);
        void stop();
        bool sendCommand(CommandBase& cmd);
        void performHandShake();
    private slots:
        void periodicHandshake();
    signals:
        void handShakeCompleted();
    };

    class ZmqEventSocket : public QObject
    {
        Q_OBJECT

        nzmqt::ZMQContext* context;
        nzmqt::ZMQSocket* socket;

    public:
        explicit ZmqEventSocket(QObject* parent = 0);
        ~ZmqEventSocket();

        // hack, not sure how to bind socket receive message.
        inline nzmqt::ZMQSocket* getSocket() {
            return socket;
        }

    public:
        void start(QString socketUrl);
        void stop();
    };
};


#endif // RBKIT_ZMQ_SOCKETS_H
