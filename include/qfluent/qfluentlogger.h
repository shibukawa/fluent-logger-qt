#ifndef QFLUENTLOGGER_H
#define QFLUENTLOGGER_H

#include <QString>
#include <QThread>
#include <QDateTime>
#include "qfluentsender.h"
#include <msgpack.hpp>

template <typename Stream>
inline msgpack::packer<Stream>& operator<< (msgpack::packer<Stream>& o, const QString& v)
{
    return msgpack::detail::packer_serializer<Stream, QString>::pack(o, v.toStdString());
}

namespace QFluent {
    class QLogger : QObject
    {
        Q_OBJECT
        QString _prefix;
        QSender* _sender;
    public:
        QLogger(const QString& prefix, const QString& host="localhost", int port=24224, float timeout=3.0, bool verbose=false);
        QLogger();
        ~QLogger();

    private:
        void add_args(msgpack::packer<msgpack::sbuffer>& packer, const QString& key, const char* value)
        {
            packer.pack(key.toStdString());
            packer.pack(QString(value).toStdString());
        }

        template<typename V>
        void add_args(msgpack::packer<msgpack::sbuffer>& packer, const QString& key, const V& value)
        {
            packer.pack(key.toStdString());
            packer.pack(value);
        }

        template<typename V, typename... Params>
        void add_args(msgpack::packer<msgpack::sbuffer>& packer, const QString& key, const V& value, Params... parameters)
        {
            packer.pack(key.toStdString());
            packer.pack(value);
            this->add_args(packer, parameters...);
        }

        template<typename... Params>
        void add_args(msgpack::packer<msgpack::sbuffer>& packer, const QString& key, const char * value, Params... parameters)
        {
            packer.pack(key.toStdString());
            packer.pack(QString(value));
            this->add_args(packer, parameters...);
        }

    public:
        template<typename... Params>
        bool log(const QString& label, Params... parameters)
        {
            return log(label, QDateTime::currentDateTime(), parameters...);
        }

        template<typename... Params>
        bool log(const QString& label, const QDateTime& timestamp, Params... parameters)
        {
            msgpack::sbuffer sbuf;
            msgpack::packer<msgpack::sbuffer> packer(sbuf);
            packer.pack_array(3);
            if( this->_prefix.size() ) {
                if( label.size() ) {
                    packer.pack((this->_prefix + "." + label).toStdString());
                }
                else {
                    packer.pack(this->_prefix.toStdString());
                }
            }
            else {
                if( label.size() ) {
                    packer.pack(label.toStdString());
                }
                else {
                    /* TODO is this an error */
                    packer.pack(std::string(""));
                }
            }
            packer.pack(timestamp.toTime_t());
            packer.pack_map(sizeof...(Params) / 2);
            this->add_args(packer, parameters...);
            return this->_sender->send(sbuf);
        }
    };
}

#endif // QFLUENTLOGGER_H
