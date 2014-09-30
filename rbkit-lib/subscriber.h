#ifndef SUBSCRIBER_H
#define SUBSCRIBER_H

#include <QObject>
#include <QVariantMap>

#include "rbevents.h"
#include "model/objectstore.h"
#include "sqlconnectionpool.h"

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
    class CommandBase;
}

class Subscriber : public QObject
{
    Q_OBJECT

    RBKit::ZmqCommandSocket* commandSocket;
    RBKit::ZmqEventSocket* eventSocket;

    // add a timer to detect command timeout, and a timer to emit stats
    QTimer* statsTimer;

    RBKit::ObjectStore *objectStore;
    RBKit::JsBridge* jsBridge;

public:
    explicit Subscriber(RBKit::JsBridge* jsBridge);
    ~Subscriber();

public:
    // function overloading
    void processEvent(const RBKit::EvtNewObject&);
    void processEvent(const RBKit::EvtDelObject&);
    void processEvent(const RBKit::EvtGcStats&);
    void processEvent(const RBKit::EvtGcStart&);
    void processEvent(const RBKit::EvtGcStop&);
    void processEvent(const RBKit::EvtObjectDump&);
    void processEvent(const RBKit::EvtCollection&);

signals:
    void disconnected();
    void connected();
    void errored(const QString &);
    void objectDumpAvailable(int snapshotVersion);

public slots:
    void startListening(QString, QString);
    void stop();
    void onMessageReceived(const QList<QByteArray>&);
    void onCommandSent(QSharedPointer<RBKit::CommandBase> cmd, bool success);
    void onStatsTimerExpiry();
    void triggerGc();
    void takeSnapshot();
};

#endif // SUBSCRIBER_H
