#ifndef SUBSCRIBER_H
#define SUBSCRIBER_H

#include <QObject>
#include <QVariantMap>

// forward declaration of nzmqt classes
namespace nzmqt
{
   class ZMQContext;
   class ZMQSocket;
}

class QTimer;

class Subscriber : public QObject
{
    Q_OBJECT

    nzmqt::ZMQContext* m_context;
    nzmqt::ZMQSocket* m_socket;

    QMap<QString, QString> m_objId2Type;
    QMap<QString, int> m_event2Count;

    // we are interested in this count.
    QVariantMap m_type2Count;

    // add a timer to emit stats
    QTimer* m_timer;

public:
    explicit Subscriber(QObject *parent = 0);
    ~Subscriber();

signals:
    void messageReady(const QVariantMap& map);
    void connected();
    void disconnected();
    void errored(const QString &);

public slots:
    void startListening(const QString &);
    void onMessageReceived(const QList<QByteArray>&);
    void onTimerExpiry();
};

#endif // SUBSCRIBER_H
