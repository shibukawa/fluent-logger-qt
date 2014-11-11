#include "qfluent/qfluentlogger.h"

QFluent::QLogger::QLogger(const QString& prefix, const QString& host, int port, float timeout, bool verbose)
    : _prefix(prefix)
{
    this->_sender = new QSender(host, port, timeout, verbose);
}

QFluent::QLogger::~QLogger()
{
    this->_sender->close();
    delete this->_sender;

}
