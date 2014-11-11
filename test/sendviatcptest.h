#ifndef SENDVIATCPTEST_H
#define SENDVIATCPTEST_H

#include <QObject>

class TestUtil;

class SendViaTCPTest : public QObject
{
    Q_OBJECT
public:
    explicit SendViaTCPTest(QObject *parent = 0);

private Q_SLOTS:
    void init();
    void sendJsonText();
    void cleanup();

protected:
    TestUtil* _util;
};

#endif // SENDVIATCPTEST_H
