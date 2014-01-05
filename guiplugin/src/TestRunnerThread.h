#ifndef TESTRUNNERTHREAD_H__
#define TESTRUNNERTHREAD_H__

#include <QThread>

class TestRunnerThread : public QThread
{
    Q_OBJECT

public:
    TestRunnerThread(void* testController, QObject* parent = 0);
    virtual ~TestRunnerThread();

    // Test::Observer members

    // virtual void onAllTestSuitesBegin(int testSuitesNumTotal);
    // virtual void onAllTestSuitesEnd(int lastTestSuiteNum, int testSuitesNumTotal,
            // int numErrs, int numExcepts);
//
    // virtual void onTestSuiteBegin(const std::string& testSuiteLabel,
            // int testSuiteNum, int testSuitesNumTotal);
//
    // virtual void onTestSuiteEnd(int numErrs);
    // virtual void onTestSuiteEndWithStdException(int numErrs, const std::exception& e);
    // virtual void onTestSuiteEndWithEllipsisException(int numErrs);
//
    // virtual void onAssertBegin(const std::string& testlabel);
    // virtual void onAssertExceptionBegin(const std::string& testlabel);
    // virtual void onAssertNoExceptionBegin(const std::string& testlabel);
//
    // virtual void onAssertEnd(bool ok);
    // virtual void onAssertExceptionEndWithExpectedException(const std::exception& e);
    // virtual void onAssertExceptionEndWithUnexpectedException(const std::exception& e);
    // virtual void onAssertExceptionEndWithEllipsisException();
    // virtual void onAssertNoExceptionEndWithStdException(const std::exception& e);
    // virtual void onAssertNoExceptionEndWithEllipsisException();

signals:
    void allTestSuitesBegin(int testSuitesNumTotal);
    void allTestSuitesEnd(int lastTestSuiteNum, int testSuitesNumTotal,
            int numErrs, int numExcepts);

    void testSuiteBegin(const std::string& testSuiteLabel,
            int testSuiteNum, int testSuitesNumTotal);

    void testSuiteEnd(int numErrs);
    void testSuiteEndWithStdException(int numErrs, const std::exception& e);
    void testSuiteEndWithEllipsisException(int numErrs);

    void assertBegin(const std::string& testlabel);
    void assertExceptionBegin(const std::string& testlabel);
    void assertNoExceptionBegin(const std::string& testlabel);

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
    void* testController;
};

#endif /* TESTRUNNERTHREAD_H */
