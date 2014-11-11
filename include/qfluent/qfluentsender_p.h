#ifndef QFLUENTSENDER_P_H
#define QFLUENTSENDER_P_H

#include <QObject>
#include <QString>
#include <QMutex>
#include <QTcpSocket>
#include <QByteArray>
#include <msgpack.hpp>

namespace QFluent {

    class QSenderPrivate : public QObject
    {
        Q_OBJECT
    protected:
        QString _host;
        int _port;
        size_t _bufferUsed;
        size_t _bufferMax;
        float _timeout;
        bool _verbose;

        QList<QByteArray> _buffer;
        QTcpSocket* _socket;

        bool _connected;
        QMutex _mutex;

    public:
        QSenderPrivate(const QString& host ="localhost", int port = 24224, float timeout = 3.0, bool verbose = false);
        ~QSenderPrivate();

    public slots:
        void process();
        void close();
        void send(const QByteArray& bytes);
        void error(QAbstractSocket::SocketError error);

    signals:
        void finished();
        void connectionFinished();

    protected:
        bool reconnect();
    };
}

#endif // QFLUENTSENDER_H
