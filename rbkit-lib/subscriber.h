#ifndef SUBSCRIBER_H
#define SUBSCRIBER_H

#include <QObject>
#include <QVariantMap>

#include "rbevents.h"

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
}

class Subscriber : public QObject
{
    Q_OBJECT

    RBKit::ZmqCommandSocket* commandSocket;
    RBKit::ZmqEventSocket* eventSocket;

    QMap<QString, QString> m_objId2Type;
    QMap<QString, int> m_event2Count;


    // we are interested in this count.
    QVariantMap m_type2Count;

    // add a timer to emit stats
    QTimer* m_timer;

public:
    explicit Subscriber(QObject *parent = 0);
    ~Subscriber();

public:
    // function overloading
    void processEvent(const RBKit::EvtNewObject&);
    void processEvent(const RBKit::EvtDelObject&);

signals:
    void messageReady(const QVariantMap& map);
    void connected();
    void disconnected();
    void errored(const QString &);

public slots:
    void startListening(QString, QString);
    void stop();
    void onMessageReceived(const QList<QByteArray>&);
    void onTimerExpiry();
};

#endif // SUBSCRIBER_H
