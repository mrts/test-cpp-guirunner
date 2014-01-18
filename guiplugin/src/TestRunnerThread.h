#ifndef TESTRUNNERTHREAD_H__
#define TESTRUNNERTHREAD_H__

#include <testcpp/testcpp.h>
#include <QThread>
#include <stdexcept>

class CancelTestRun : public std::exception
{};

class TestRunnerThread : public QThread,
                         public Test::Observer
{
    Q_OBJECT

public:
    TestRunnerThread(void* testController, QObject* parent = 0);
    virtual ~TestRunnerThread();

    void requestCancel()
    { cancelRequested = true; }

    // Test::Observer members

    virtual void onAllTestSuitesBegin(int testSuitesNumTotal)
    {
        exitIfCancelRequested();
        emit allTestSuitesBegin(testSuitesNumTotal);
    }
    virtual void onAllTestSuitesEnd(int lastTestSuiteNum,
            int testSuitesNumTotal, int numErrs, int numExcepts)
    {
        exitIfCancelRequested();
        emit allTestSuitesEnd(lastTestSuiteNum, testSuitesNumTotal,
                numErrs, numExcepts);
    }

    virtual void onTestSuiteBegin(const std::string& testSuiteLabel,
            int testSuiteNum, int testSuitesNumTotal)
    {
        exitIfCancelRequested();
        emit testSuiteBegin(testSuiteLabel, testSuiteNum, testSuitesNumTotal);
    }

    virtual void onTestSuiteEnd(int numErrs)
    {
        exitIfCancelRequested();
        emit testSuiteEnd(numErrs);
    }
    virtual void onTestSuiteEndWithStdException(int numErrs, const std::exception& e)
    {
        exitIfCancelRequested();
        emit testSuiteEndWithStdException(numErrs, e);
    }
    virtual void onTestSuiteEndWithEllipsisException(int numErrs)
    {
        exitIfCancelRequested();
        emit testSuiteEndWithEllipsisException(numErrs);
    }

    virtual void onAssertBegin(const std::string& testlabel,
            const char* const function, const char* const file, int line)
    {
        exitIfCancelRequested();
        emit assertBegin(testlabel, function, file, line);
    }
    virtual void onAssertExceptionBegin(const std::string& testlabel,
            const char* const function, const char* const file, int line)
    {
        exitIfCancelRequested();
        emit assertExceptionBegin(testlabel, function, file, line);
    }
    virtual void onAssertNoExceptionBegin(const std::string& testlabel,
            const char* const function, const char* const file, int line)
    {
        exitIfCancelRequested();
        emit assertNoExceptionBegin(testlabel, function, file, line);
    }

    virtual void onAssertEnd(bool ok)
    {
        exitIfCancelRequested();
        emit assertEnd(ok);
    }
    virtual void onAssertExceptionEndWithExpectedException(const std::exception& e)
    {
        exitIfCancelRequested();
        emit assertExceptionEndWithExpectedException(e);
    }
    virtual void onAssertExceptionEndWithUnexpectedException(const std::exception& e)
    {
        exitIfCancelRequested();
        emit assertExceptionEndWithUnexpectedException(e);
    }
    virtual void onAssertExceptionEndWithEllipsisException()
    {
        exitIfCancelRequested();
        emit assertExceptionEndWithEllipsisException();
    }
    virtual void onAssertNoExceptionEndWithStdException(const std::exception& e)
    {
        exitIfCancelRequested();
        emit assertNoExceptionEndWithStdException(e);
    }
    virtual void onAssertNoExceptionEndWithEllipsisException()
    {
        exitIfCancelRequested();
        emit assertNoExceptionEndWithEllipsisException();
    }

signals:
    void looseStdExceptionDuringTestRun(const std::exception& e);
    void looseEllipsisExceptionDuringTestRun();

    void allTestSuitesBegin(int testSuitesNumTotal);
    void allTestSuitesEnd(int lastTestSuiteNum, int testSuitesNumTotal,
            int numErrs, int numExcepts);

    void testSuiteBegin(const std::string& testSuiteLabel,
            int testSuiteNum, int testSuitesNumTotal);

    void testSuiteEnd(int numErrs);
    void testSuiteEndWithStdException(int numErrs, const std::exception& e);
    void testSuiteEndWithEllipsisException(int numErrs);

    void assertBegin(const std::string& testlabel,
            const char* function, const char* file, int line);
    void assertExceptionBegin(const std::string& testlabel,
            const char* function, const char* file, int line);
    void assertNoExceptionBegin(const std::string& testlabel,
            const char* function, const char* file, int line);

    void assertEnd(bool ok);
    void assertExceptionEndWithExpectedException(const std::exception& e);
    void assertExceptionEndWithUnexpectedException(const std::exception& e);
    void assertExceptionEndWithEllipsisException();
    void assertNoExceptionEndWithStdException(const std::exception& e);
    void assertNoExceptionEndWithEllipsisException();

protected:
    // QThread override
    virtual void run();

private:
    inline void exitIfCancelRequested()
    {
        if (cancelRequested)
            throw CancelTestRun();
    }

    void* testController;
    static bool cancelRequested;
};

#endif /* TESTRUNNERTHREAD_H */
