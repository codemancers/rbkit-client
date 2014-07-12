#include <QDebug>
#include <QThread>
#include <QTimer>

#include "nzmqt/nzmqt.hpp"

#include <msgpack.hpp>

#include "subscriber.h"

static const int rbkcZmqTotalIoThreads = 1;
static const int timerIntervalInMs = 2000;

Subscriber::Subscriber(QObject *parent) :
    QObject(parent)
{
    // second argument to creating a context is number of io threads. right
    // now, we are using only 1 thread, so defaulting to 1 for now.
    m_context = new nzmqt::SocketNotifierZMQContext(this, rbkcZmqTotalIoThreads);
    m_socket = m_context->createSocket(nzmqt::ZMQSocket::TYP_SUB, this);
    m_socket->setOption(nzmqt::ZMQSocket::OPT_SUBSCRIBE, "", 0);

    connect(m_socket, SIGNAL(messageReceived(const QList<QByteArray>&)),
            this, SLOT(onMessageReceived(const QList<QByteArray>&)));

    // initialize the timer, mark it a periodic one, and connect to timeout.
    m_timer = new QTimer(this);
    m_timer->setInterval(timerIntervalInMs);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(onTimerExpiry()));
}

Subscriber::~Subscriber()
{
    m_socket->close();
    delete m_socket;
    delete m_context;
    emit disconnected();
}

void Subscriber::startListening(const QString& host)
{
    qDebug() << "Got " << host;
    QByteArray ba = host.toLocal8Bit();
    const char *hostString = ba.data();
    try
    {
        m_socket->connectTo(hostString);
    }
    catch(zmq::error_t err)
    {
        QString str = QString::fromUtf8(err.what());
        qDebug() << str ;
        emit  errored(str);
    }

    emit connected();

    qDebug() << m_context->isStopped();
    if (!m_context->isStopped()) {
        m_context->start();
        m_timer->start();
    }
    qDebug() << "started";

}


void Subscriber::onMessageReceived(const QList<QByteArray>& rawMessage)
{
    for (QList<QByteArray>::ConstIterator iter = rawMessage.begin();
         rawMessage.end() != iter; ++iter)
    {
        const QByteArray& message = *iter;
        msgpack::unpacked unpackedMessage;
        msgpack::unpack(&unpackedMessage, (const char *)message.data(), message.size());
        msgpack::object_raw rawMessage = unpackedMessage.get().via.raw;
        QString strMessage = QString::fromUtf8(rawMessage.ptr, rawMessage.size);
        QStringList eventInfo = strMessage.split(QChar(' '));

        if (eventInfo.length() > 2) {

            // just for recording purpose, we are not using it anywhere
            m_objId2Type[eventInfo[2]] = eventInfo[1];
            ++m_event2Count[eventInfo[0]];

            // initialize the count if not valid
            if (!m_type2Count[eventInfo[1]].isValid())
                m_type2Count[eventInfo[1]] = QVariant(0);

            // increment or decrement the count according the event
            if (!eventInfo[0].compare(QString("obj_created"))) {
                int value = m_type2Count[eventInfo[1]].toInt() + 1;
                m_type2Count[eventInfo[1]].setValue(value);
            } else {
                int oldCount = m_type2Count[eventInfo[1]].toInt();
                int value;
                if(oldCount > 0) {
                    value = oldCount - 1;
                } else {
                    value = 0;
                }
                m_type2Count[eventInfo[1]].setValue(value);
            }

        }
    }
}


void Subscriber::onTimerExpiry()
{
    qDebug() << m_type2Count;
    emit messageReady(m_type2Count);
}
