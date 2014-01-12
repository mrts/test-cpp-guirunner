#ifndef TESTPROGRESSDIALOG_H__
#define TESTPROGRESSDIALOG_H__

#include <QDialog>

namespace Ui {
class TestProgressDialog;
}

class TestRunnerThread;
class QTreeWidgetItem;

class TestProgressDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TestProgressDialog(void* testController, QWidget *parent = 0);
    ~TestProgressDialog();

private slots:
    void onDetails();
    void onCancel();
    void onOK();

    void onTestRunnerFinished();
    void onTestRunnerTerminated();

    void onLooseStdExceptionDuringTestRun(const std::exception& e);
    void onLooseEllipsisExceptionDuringTestRun();

    void onAllTestSuitesBegin(int testSuitesNumTotal);
    void onAllTestSuitesEnd(int lastTestSuiteNum, int testSuitesNumTotal,
            int numErrs, int numExcepts);

    void onTestSuiteBegin(const std::string& testSuiteLabel,
            int testSuiteNum, int testSuitesNumTotal);

    void onTestSuiteEnd(int numErrs);
    void onTestSuiteEndWithStdException(int numErrs, const std::exception& e);
    void onTestSuiteEndWithEllipsisException(int numErrs);

    void onAssertBegin(const std::string& testlabel);
    void onAssertExceptionBegin(const std::string& testlabel);
    void onAssertNoExceptionBegin(const std::string& testlabel);

    void onAssertEnd(bool ok);
    void onAssertExceptionEndWithExpectedException(const std::exception& e);
    void onAssertExceptionEndWithUnexpectedException(const std::exception& e);
    void onAssertExceptionEndWithEllipsisException();
    void onAssertNoExceptionEndWithStdException(const std::exception& e);
    void onAssertNoExceptionEndWithEllipsisException();

private:
    void flipDetails();

    Ui::TestProgressDialog* ui;
    TestRunnerThread* testRunnerThread;
    bool detailsHidden;

    QTreeWidgetItem* currentSuite;
    QTreeWidgetItem* currentAssert;
};

#endif /* TESTPROGRESSDIALOG_H */
