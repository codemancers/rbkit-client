#ifndef SUBSCRIBER_H
#define SUBSCRIBER_H

#include <QObject>
#include <QVariantMap>

#include "rbevents.h"
#include "model/objectstore.h"
#include "sqlconnectionpool.h"
#include "model/cpuprof.h"

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

    quint64 messageCounter;

    CpuProf cpuProf;

public:
    explicit Subscriber(RBKit::JsBridge* jsBridge);
    ~Subscriber();

public:
    // function overloading
    void processEvent(const RBKit::EvtNewObject&);
    void processEvent(const RBKit::EvtDelObject&);
    void processEvent(const RBKit::EvtGcStats&);
    void processEvent(const RBKit::EvtGcStart&);
    void processEvent(const RBKit::EvtGcStartM&);
    void processEvent(const RBKit::EvtGcStop&);
    void processEvent(const RBKit::EvtObjectDump&);
    void processEvent(const RBKit::EvtCollection&);
    void processEvent(const RBKit::EvtHandshake&handShake);
    void processEvent(const RBKit::EvtCpuSample&);
    void performHandshake();
    void handShakeCompleted();
    void emitConnectionError(QString message);
    nzmqt::ZMQContext *getContext() const;
    void setContext(nzmqt::ZMQContext *value);

signals:
    void disconnected();
    void connected();
    void errored(const QString &);
    void youngGenStats(QVariantMap map);
    void secondGenStats(QVariantMap map);
    void oldGenStats(QVariantMap map);
    void objectDumpAvailable(int snapshotVersion);

public slots:
    void startListening(QString _commandsUrl, QString _eventsUrl);
    void stop();
    void onMessageReceived(const QList<QByteArray>&);
    void onTimerExpiry();
    void triggerGc();
    void startCPUProfiling();
    void stopCPUProfiling();
    void takeSnapshot();
    void startSubscriber();

private:
    void checkForMissingMessages(const quint64 counter);
};

#endif // SUBSCRIBER_H
