#include "testrunner.h"
#include <QtTest>

#include "test/sendviatcptest.h"

TestRunner::TestRunner(int argc, char **argv, QObject *parent)
    : QObject(parent), _argc(argc), _argv(argv) {}

TestRunner::~TestRunner() {}

void TestRunner::run()
{
    int status = 0;
    auto ASSERT_TEST = [&status, this](QObject* obj) {
       status |= QTest::qExec(obj, this->_argc, this->_argv);
       delete obj;
    };

    ASSERT_TEST(new SendViaTCPTest());

    QCoreApplication::instance()->exit(status);
}
