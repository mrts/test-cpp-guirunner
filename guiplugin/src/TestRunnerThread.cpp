#include "TestRunnerThread.h"

TestRunnerThread::TestRunnerThread(void* controller, QObject* parent) :
    QThread(parent),
    testController(controller)
{}

TestRunnerThread::~TestRunnerThread()
{}

void TestRunnerThread::run()
{
    // Test::Controller* controller = reinterpret_cast<Test::Controller*>(testController);
    // controller->setIndependentObserver(this); <-- will not delete in ~
    // controller->run();

    emit allTestSuitesBegin(10);

    for (int i=0; i < 11; ++i) {
        emit testSuiteBegin("Test suite", i, 0);
        emit assertBegin("Assert1");
        emit assertEnd(true);
        emit assertBegin("Assert2");
        emit assertEnd(i < 5);
        sleep(1);
    }
}
