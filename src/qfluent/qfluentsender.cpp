#include <QWriteLocker>
#include <QEventLoop>
#include <QThread>
#include <QMetaObject>
#include "qfluent/qfluentsender.h"
#include "qfluent/qfluentsender_p.h"

QFluent::QSender::QSender(const QString& host, int port, float timeout, bool verbose)
{
    this->_sender = new QSenderPrivate(host, port, timeout, verbose);
    this->_thread = new QThread();
    connect(this->_thread, SIGNAL(started()), this->_sender, SLOT(process()));
    connect(this->_sender, SIGNAL(finished()), this->_thread, SLOT(quit()));
    this->_sender->moveToThread(this->_thread);
    this->_thread->start();
}

QFluent::QSender::~QSender()
{
    this->close();
    delete this->_sender;
    delete this->_thread;
}

bool QFluent::QSender::send(const msgpack::sbuffer& sbuf)
{
    if (this->_thread->isRunning()) {
        QByteArray array(sbuf.data(), sbuf.size());
        QMetaObject::invokeMethod(this->_sender, "send", Qt::QueuedConnection, Q_ARG(QByteArray, array));
        return true;
    }
    return false;
}

void QFluent::QSender::close()
{
    if (this->_thread->isRunning()) {
        QMetaObject::invokeMethod(this->_sender, "close", Qt::QueuedConnection);
        while (this->_thread->isRunning()) {
            this->_thread->wait(1000);
            qDebug() << "wait finish";
        }
    }
}
