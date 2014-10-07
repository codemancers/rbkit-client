#ifndef RBKIT_ZMQ_SOCKETS_H
#define RBKIT_ZMQ_SOCKETS_H

#include <QObject>
#include <QTimer>

// forward declaration of nzmqt classes.
namespace nzmqt
{
   class ZMQContext;
   class ZMQSocket;
   class SocketNotifierZMQContext;
}


namespace RBKit
{
    class CommandBase;

    class ZmqCommandSocket : public QObject
    {
        Q_OBJECT

        nzmqt::ZMQContext* context;
        nzmqt::ZMQSocket* socket;

    public:
        explicit ZmqCommandSocket(QObject* parent, nzmqt::ZMQContext *_context);
        ~ZmqCommandSocket();

    public:
        void start(QString socketUrl);
        void stop();
        bool sendCommand(CommandBase& cmd);
        bool performHandShake();
    signals:
        void handShakeCompleted();
    };

    class ZmqEventSocket : public QObject
    {
        Q_OBJECT

        nzmqt::ZMQContext* context;
        nzmqt::ZMQSocket* socket;

    public:
        explicit ZmqEventSocket(QObject* parent, nzmqt::ZMQContext *_context);
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
