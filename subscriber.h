#ifndef SUBSCRIBER_H
#define SUBSCRIBER_H

#include <QObject>
#include <QVariantMap>

#include <zmq.hpp>

class Subscriber : public QObject
{
    Q_OBJECT

    zmq::socket_t *socket;
    zmq::context_t *context;

    QMap<QString, QString> m_objId2Type;
    QMap<QString, int> m_event2Count;

    // we are interested in this count.
    QVariantMap m_type2Count;

public:
    explicit Subscriber(QObject *parent = 0);
    ~Subscriber();

signals:
    void messageReady(const QVariantMap& map);
    void connected();
    void disconnected();
    void errored(const QString &);

public slots:
    void startListening();
};

#endif // SUBSCRIBER_H
