#ifndef SUBSCRIBER_H
#define SUBSCRIBER_H

#include <QObject>

#include <zmq.hpp>

class Subscriber : public QObject
{
    Q_OBJECT

    zmq::socket_t *socket;
    zmq::context_t *context;

public:
    explicit Subscriber(QObject *parent = 0);
    ~Subscriber();

signals:
    void messageReady(const QString &);
    void connected();
    void disconnected();
    void errored(const QString &);

public slots:
    void startListening();
};

#endif // SUBSCRIBER_H
