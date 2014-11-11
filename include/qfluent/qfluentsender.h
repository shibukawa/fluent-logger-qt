#ifndef QFLUENTSENDER_H
#define QFLUENTSENDER_H

#include <QObject>
#include <QString>
#include <QThread>
#include <msgpack.hpp>

namespace QFluent {

    class QSenderPrivate;

    class QSender : public QObject
    {
        Q_OBJECT
    protected:
        QSenderPrivate* _sender;
        QThread* _thread;

    public:
        QSender(const QString& host ="localhost", int port = 24224, float timeout = 3.0, bool verbose = false);
        ~QSender();

        bool send(const ::msgpack::sbuffer& sbuf);
        void close();

    signals:
        void finished();
    };
}

#endif // QFLUENTSENDER_H
