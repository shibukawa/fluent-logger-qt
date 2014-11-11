#include "sendviatcptest.h"
#include "testutil.h"
#include "qfluent/qfluentlogger.h"
#include <QDebug>

const char* ikpath = "/Users/shibukawa.yoshiki/gobin/ik";

SendViaTCPTest::SendViaTCPTest(QObject *parent) :
    QObject(parent)
{
}

void SendViaTCPTest::init()
{
}

void SendViaTCPTest::sendJsonText()
{
    TestUtil* util = new TestUtil;
    util->launchIk(ikpath);
    QFluent::QLogger logger("prefix", "localhost", 24224, 3.0, true);
    logger.log("title", "key", "value");
    qDebug() << "finish sending";
    auto result = util->waitCloseIk();
    qDebug() << result;
}

void SendViaTCPTest::cleanup()
{
}
