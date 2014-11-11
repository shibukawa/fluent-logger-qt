#include <QWriteLocker>
#include <QEventLoop>
#include "qfluent/qfluentsender_p.h"
#include <iostream>
#include <QThread>

QFluent::QSenderPrivate::QSenderPrivate(const QString& host, int port, float timeout, bool verbose)
    : _host(host), _port(port), _timeout(timeout), _verbose(verbose), _connected(false)
{
}

QFluent::QSenderPrivate::~QSenderPrivate()
{
    this->_socket->deleteLater();
}

void QFluent::QSenderPrivate::send(const QByteArray& bytes)
{
    if (this->reconnect()) {
        this->_socket->write(bytes);
        this->_socket->waitForBytesWritten();
        qDebug() << bytes;
        if (this->_verbose) {
            std::cout << "sending log" << std::endl;
        }
    }
}

void QFluent::QSenderPrivate::error(QAbstractSocket::SocketError error)
{
    qDebug() << "QSenderPrivate::error:" << this->_socket->errorString();
}

bool QFluent::QSenderPrivate::reconnect()
{
    qDebug() << "QSenderPrivate::reconnect()";
    if (this->_socket->state() == QAbstractSocket::UnconnectedState) {
        this->_socket->connectToHost(this->_host, this->_port, QIODevice::WriteOnly);
        if (!this->_socket->waitForConnected(this->_timeout * 1000)) {
            std::cerr << "connection error: " << this->_socket->errorString().toStdString() << std::endl;
            return false;
        }
        if (this->_verbose) {
            std::cout << "connected" << std::endl;
        }
    }
    return true;
}

void QFluent::QSenderPrivate::close()
{
    qDebug() << "QSenderPrivate::close()";
    if (this->_socket->state() != QAbstractSocket::UnconnectedState) {
        this->_socket->disconnect();
        if (!this->_socket->waitForDisconnected(this->_timeout * 1000)) {
            std::cerr << this->_socket->errorString().toStdString() << std::endl;
        }
    }
    qDebug() << "disconnection finished";
    emit connectionFinished();
}


void QFluent::QSenderPrivate::process()
{
    this->_socket = new QTcpSocket();
    connect(this->_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(error(QAbstractSocket::SocketError)));
    if (this->reconnect()) {
        qDebug() << "start event loop";
        QEventLoop loop;
        QObject::connect(this, SIGNAL(connectionFinished()), &loop, SLOT(quit()));
        // start event loop
        loop.exec();
        qDebug() << "finish event loop";
    }
    qDebug() << "finished";
    emit finished();
    QThread::currentThread()->exit(0);
}
