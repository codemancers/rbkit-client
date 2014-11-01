#ifndef SUBSCRIBER_H
#define SUBSCRIBER_H

#include <QObject>
#include <QVariantMap>
#include <QThread>

#include "rbevents.h"
#include "model/objectstore.h"
#include "sqlconnectionpool.h"

// forward declaration of nzmqt classes
namespace nzmqt
{
   class ZMQContext;
   class ZMQSocket;
   class ZSocketNotifierZMQContext;
}

class QTimer;

namespace RBKit
{
    class ZmqCommandSocket;
    class ZmqEventSocket;
    class RbHeapWorker;
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
    QString commandUrl;
    QString eventServerUrl;
    nzmqt::ZMQContext *context;
    bool connectionEstablished;

    QSharedPointer<RBKit::RbHeapWorker> heapWorker;
    QThread heapDumpThread;

public:
    explicit Subscriber(RBKit::JsBridge* jsBridge);
    ~Subscriber();

    void setup();               // 2nd constructor

public:
    // function overloading
    void processEvent(const RBKit::EvtNewObject&);
    void processEvent(const RBKit::EvtDelObject&);
    void processEvent(const RBKit::EvtGcStats&);
    void processEvent(const RBKit::EvtGcStart&);
    void processEvent(const RBKit::EvtGcStop&);
    void processEvent(const RBKit::EvtObjectDump&);
    void processEvent(const RBKit::EvtCollection&);
    void performHandshake();
    void handShakeCompleted();
    void emitConnectionError(QString message);
    nzmqt::ZMQContext *getContext() const;
    void setContext(nzmqt::ZMQContext *value);

signals:
    void disconnected();
    void connected();
    void errored(const QString &);
    void objectDumpAvailable(int snapshotVersion);
    void dumpReceived(const QByteArray);

public slots:
    void startListening(QString _commandsUrl, QString _eventsUrl);
    void stop();
    void onMessageReceived(const QList<QByteArray>&);
    void onTimerExpiry();
    void triggerGc();
    void takeSnapshot();
    void startSubscriber();
};

#endif // SUBSCRIBER_H
