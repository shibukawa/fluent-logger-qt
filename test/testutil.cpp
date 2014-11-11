#include "testutil.h"
#include <QDir>
#include <QTextStream>
#include <QDateTime>
#include <QDebug>
#include <stdexcept>
#include <QEventLoop>
#include "externalprocess.h"
#include <QTimer>

TestUtil::TestUtil() : _process(nullptr)
{
}

TestUtil::~TestUtil()
{
    if (this->_process) {
        this->_process->waitForFinished(-1);
        delete this->_process;
    }
    if (!this->_testDir.isEmpty() and !this->_skipRemove) {
        QDir tempdir(this->_testDir);
        tempdir.removeRecursively();
    }
}

QDir TestUtil::makeTestDir(bool skipRemove)
{
    this->_skipRemove = this->_skipRemove || skipRemove;
    if (!this->_testDir.isNull()) {
        return QDir(this->_testDir);
    }
    QDir temp = QDir::temp();
    QString dirname = QString::number(QDateTime::currentMSecsSinceEpoch());
    temp.mkdir(dirname);
    this->_testDir = temp.filePath(dirname);
    if (skipRemove) {
        qDebug() << "test dir:" << this->_testDir;
    }
    return QDir(this->_testDir);
}

void TestUtil::makeFile(const QString filename, const QString content)
{
    if (this->_testDir.isEmpty()) {
        throw new std::logic_error("temp dir is not initialized");
    }
    QDir tempdir(this->_testDir);
    QString filepath = tempdir.filePath(filename);
    QFile file(filepath);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        throw new std::logic_error("file creation error");
    }
    QTextStream stream(&file);
    stream << content << endl;
    file.close();
}

QString TestUtil::readFile(const QString &filename)
{
    if (this->_testDir.isEmpty()) {
        throw new std::logic_error("temp dir is not initialized");
    }
    QDir tempdir(this->_testDir);
    QString filepath = tempdir.filePath(filename);
    QFile file(filepath);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        throw new std::logic_error("file creation error");
    }
    QTextStream stream(&file);
    QString result = stream.readAll();
    file.close();
    return result;
}

void TestUtil::launchIk(const QString &path)
{
    auto dir = this->makeTestDir(true);
    dir.mkdir("config");
    dir.mkdir("log");
    auto configPath = dir.filePath("config/fluent.conf");
    this->makeFile(configPath, \
        QString("<source>\n"
                "  type forward\n"
                "  port 24224\n"
                "</source>\n"
                "<match **>\n"
                "  type file\n"
                "  path %1/log/*.log\n"
                "</match>").arg(dir.path()));

    QEventLoop loop;
    QTimer::singleShot(200, &loop, SLOT(quit()));
    qDebug() << "start wait";
    loop.exec();
    qDebug() << "finish wait";

    QStringList param;
    param << "-c" << configPath;
    this->_process = new ExternalProcess(true, true, this);
    this->_process->run(path, param);

    QTimer::singleShot(500, &loop, SLOT(quit()));
    qDebug() << "start wait booting";
    loop.exec();
    qDebug() << "finish wait booting";
}

QStringList TestUtil::waitCloseIk()
{
    qDebug() << "wait close";
    this->_process->terminate();
    this->_process->waitForFinished(-1);
    qDebug() << "stdout:" << this->_process->stdoutResult();
    qDebug() << "stderr:" << this->_process->stderrResult();
    QEventLoop loop;
    QTimer::singleShot(200, &loop, SLOT(quit()));
    qDebug() << "start wait";
    loop.exec();
    qDebug() << "finish wait";
    QDir tempdir(this->_testDir);

    QStringList result;
    for (const QFileInfo& info : tempdir.entryList(QStringList() << ".log")) {
        qDebug() << info.path();
    }
    return result;
}
