#ifndef TESTUTIL_H
#define TESTUTIL_H

#include <QString>
#include <QDir>

class ExternalProcess;

class TestUtil : QObject
{
    Q_OBJECT
public:
    TestUtil();
    ~TestUtil();

    QDir makeTestDir(bool skipRemove=false);
    void makeFile(const QString filename, const QString content);
    QString readFile(const QString& filename);
    void launchIk(const QString& path);
    QStringList waitCloseIk();

private:
    QString _testDir;
    bool _skipRemove;
    ExternalProcess* _process;
};

#endif // TESTUTIL_H
