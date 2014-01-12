#include "TestRunnerThread.h"

TestRunnerThread::TestRunnerThread(void* controller, QObject* parent) :
    QThread(parent),
    testController(controller)
{}

TestRunnerThread::~TestRunnerThread()
{}

bool TestRunnerThread::cancelRequested = false;

void TestRunnerThread::run()
{
    try {
        Test::Controller* controller = reinterpret_cast<Test::Controller*>(testController);
        if (!controller)
            throw std::runtime_error("Controller is null");
        controller->setObserver(this, false); // will not transfer ownership
        controller->run();
    } catch (const CancelTestRun&) {
        // swallow CancelTestRun as it is used for canceling tests
    } catch (const std::exception& e) {
        emit looseStdExceptionDuringTestRun(e);
    } catch (...) {
        emit looseEllipsisExceptionDuringTestRun();
    }
}
