#ifndef SUBSCRIBER_H
#define SUBSCRIBER_H

#include <QObject>
#include <QVariantMap>

#include "rbevents.h"
#include "objectstore.h"

// forward declaration of nzmqt classes
namespace nzmqt
{
   class ZMQContext;
   class ZMQSocket;
}

class QTimer;

namespace RBKit
{
    class ZmqCommandSocket;
    class ZmqEventSocket;
    class JsBridge;
}

class Subscriber : public QObject
{
    Q_OBJECT

    RBKit::ZmqCommandSocket* commandSocket;
    RBKit::ZmqEventSocket* eventSocket;

    // add a timer to emit stats
    QTimer* m_timer;
    RBKit::ObjectStore *objectStore;

    RBKit::JsBridge* jsBridge;

public:
    explicit Subscriber(QObject *parent, RBKit::JsBridge* jsBridge);
    ~Subscriber();

public:
    // function overloading
    void processEvent(const RBKit::EvtNewObject&);
    void processEvent(const RBKit::EvtDelObject&);
    void processEvent(const RBKit::EvtGcStats&);

signals:
    void disconnected();
    void connected();
    void errored(const QString &);

public slots:
    void startListening(QString, QString);
    void stop();
    void onMessageReceived(const QList<QByteArray>&);
    void onTimerExpiry();
    void triggerGc();
};

#endif // SUBSCRIBER_H
