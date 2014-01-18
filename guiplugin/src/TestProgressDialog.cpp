#include "TestProgressDialog.h"
#include "ui_TestProgressDialog.h"

#include "TestRunnerThread.h"

#include <QMessageBox>
#include <string>

namespace
{
const QSize COLLAPSED_SIZE = QSize(413, 90);
const QSize EXPANDED_SIZE  = QSize(413, 486);

const char* const GREEN_BAR_STYLE =
    "QProgressBar::chunk {background-color: green; border-radius: 4px;}"
    "QProgressBar {border: 1px solid grey; border-radius: 4px; "
    "text-align: center; background: #eee;}";
const char* const RED_BAR_STYLE =
    "QProgressBar::chunk {background-color: red; border-radius: 4px;}"
    "QProgressBar {border: 1px solid grey; border-radius: 4px; "
    "text-align: center; background: #eee;}";

const QBrush RED_BRUSH = QBrush(QColor(Qt::red));
}

TestProgressDialog::TestProgressDialog(void* testController, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TestProgressDialog),
    testRunnerThread(new TestRunnerThread(testController)),
    detailsHidden(true),
    currentSuite(NULL),
    currentAssert(NULL)
{
    ui->setupUi(this);

    ui->progressBar->setStyleSheet(GREEN_BAR_STYLE);
    ui->progressBar->setValue(0);

    connect(ui->detailsButton, SIGNAL(clicked()), SLOT(onDetails()));
    connect(ui->cancelButton,  SIGNAL(clicked()), SLOT(onCancel()));
    connect(ui->okButton,      SIGNAL(clicked()), SLOT(onOK()));

    onDetails();

    qRegisterMetaType<std::string>("std::string");
    qRegisterMetaType<std::exception>("std::exception");

    connect(testRunnerThread, SIGNAL(finished()),
           SLOT(onTestRunnerFinished()));
    connect(testRunnerThread, SIGNAL(terminated()),
           SLOT(onTestRunnerTerminated()));

    connect(testRunnerThread, SIGNAL(looseStdExceptionDuringTestRun(const std::exception&)),
           SLOT(onLooseStdExceptionDuringTestRun(const std::exception&)));
    connect(testRunnerThread, SIGNAL(looseEllipsisExceptionDuringTestRun()),
           SLOT(onLooseEllipsisExceptionDuringTestRun()));

    connect(testRunnerThread, SIGNAL(allTestSuitesBegin(int)),
           SLOT(onAllTestSuitesBegin(int)));
    connect(testRunnerThread, SIGNAL(allTestSuitesEnd(int, int, int, int)),
           SLOT(onAllTestSuitesEnd(int, int, int, int)));

    connect(testRunnerThread, SIGNAL(testSuiteBegin(const std::string&, int, int)),
           SLOT(onTestSuiteBegin(const std::string&, int, int)));

    connect(testRunnerThread, SIGNAL(testSuiteEnd(int)),
           SLOT(onTestSuiteEnd(int)));
    connect(testRunnerThread, SIGNAL(testSuiteEndWithStdException(int, const std::exception&)),
           SLOT(onTestSuiteEndWithStdException(int, const std::exception&)));
    connect(testRunnerThread, SIGNAL(testSuiteEndWithEllipsisException(int)),
           SLOT(onTestSuiteEndWithEllipsisException(int)));

    connect(testRunnerThread, SIGNAL(assertBegin(const std::string&,
                    const char*, const char*, int)),
           SLOT(onAssertBegin(const std::string&)));
    connect(testRunnerThread, SIGNAL(assertExceptionBegin(const std::string&,
                    const char*, const char*, int)),
           SLOT(onAssertExceptionBegin(const std::string&)));
    connect(testRunnerThread, SIGNAL(assertNoExceptionBegin(const std::string&,
                    const char*, const char*, int)),
           SLOT(onAssertNoExceptionBegin(const std::string&)));

    connect(testRunnerThread, SIGNAL(assertEnd(bool)),
           SLOT(onAssertEnd(bool)));
    connect(testRunnerThread, SIGNAL(assertExceptionEndWithExpectedException(const std::exception&)),
           SLOT(onAssertExceptionEndWithExpectedException(const std::exception&)));
    connect(testRunnerThread, SIGNAL(assertExceptionEndWithUnexpectedException(const std::exception&)),
           SLOT(onAssertExceptionEndWithUnexpectedException(const std::exception&)));
    connect(testRunnerThread, SIGNAL(assertExceptionEndWithEllipsisException()),
           SLOT(onAssertExceptionEndWithEllipsisException()));
    connect(testRunnerThread, SIGNAL(assertNoExceptionEndWithStdException(const std::exception&)),
           SLOT(onAssertNoExceptionEndWithStdException(const std::exception&)));
    connect(testRunnerThread, SIGNAL(assertNoExceptionEndWithEllipsisException()),
           SLOT(onAssertNoExceptionEndWithEllipsisException()));

    testRunnerThread->start();
}

TestProgressDialog::~TestProgressDialog()
{
    delete ui;
    delete testRunnerThread;
}

void TestProgressDialog::onDetails()
{
    ui->treeWidget->setHidden(detailsHidden);
    resize(detailsHidden ? COLLAPSED_SIZE : EXPANDED_SIZE);
    detailsHidden = !detailsHidden;
}

void TestProgressDialog::onOK()
{
    accept();
}

void TestProgressDialog::onCancel()
{
    ui->cancelButton->setHidden(true);
    testRunnerThread->requestCancel();
    testRunnerThread->wait();
}

void TestProgressDialog::onTestRunnerFinished()
{
    ui->cancelButton->setHidden(true);
    ui->okButton->setEnabled(true);
    ui->okButton->setFocus();
}

void TestProgressDialog::onTestRunnerTerminated()
{
    onTestRunnerFinished();
}

void TestProgressDialog::onLooseStdExceptionDuringTestRun(const std::exception& e)
{
    const std::string &exceptionType(typeid(e).name());
    QString message("%1 exception occurred during test run: %2");
    QMessageBox::critical(this,
            "Test runner error",
            message.arg(QString::fromStdString(exceptionType))
                   .arg(QString::fromStdString(e.what())));
}

void TestProgressDialog::onLooseEllipsisExceptionDuringTestRun()
{
    QMessageBox::critical(this,
            "Test runner error",
            "Unknown exception occurred during test run");
}

void TestProgressDialog::onAllTestSuitesBegin(int testSuitesNumTotal)
{
    ui->progressBar->setRange(0, testSuitesNumTotal);
}

void TestProgressDialog::onAllTestSuitesEnd(int /* lastTestSuiteNum: unused */, int /* testSuitesNumTotal: unused */,
            int /* numErrs: unused */, int /* numExcepts: unused */)
{
}


void TestProgressDialog::onTestSuiteBegin(const std::string& testSuiteLabel,
            int testSuiteNum, int /* testSuitesNumTotal: unused */)
{
    ui->progressBar->setValue(testSuiteNum);
    currentSuite = new QTreeWidgetItem(
            QStringList(QString::fromStdString(testSuiteLabel)));
    ui->treeWidget->addTopLevelItem(currentSuite);
}


void TestProgressDialog::onTestSuiteEnd(int numErrs)
{
    Q_ASSERT(currentSuite);
    if (numErrs > 0)
        currentSuite->setForeground(0, RED_BRUSH);
}

void TestProgressDialog::onTestSuiteEndWithStdException(int /* numErrs: unused */, const std::exception& /* e: unused */)
{
}

void TestProgressDialog::onTestSuiteEndWithEllipsisException(int /* numErrs: unused */)
{
}


void TestProgressDialog::onAssertBegin(const std::string& testlabel)
{
    Q_ASSERT(currentSuite);
    currentAssert = new QTreeWidgetItem(
            QStringList(QString::fromStdString(testlabel)));
    currentSuite->addChild(currentAssert);
}

void TestProgressDialog::onAssertExceptionBegin(const std::string& testlabel)
{
    onAssertBegin(testlabel);
}

void TestProgressDialog::onAssertNoExceptionBegin(const std::string& testlabel)
{
    onAssertBegin(testlabel);
}


void TestProgressDialog::onAssertEnd(bool ok)
{
    Q_ASSERT(currentSuite);
    Q_ASSERT(currentAssert);
    if (!ok) {
        currentSuite->setForeground(0, RED_BRUSH);
        currentAssert->setForeground(0, RED_BRUSH);
        ui->progressBar->setStyleSheet(RED_BAR_STYLE);
    }
}

void TestProgressDialog::onAssertExceptionEndWithExpectedException(const std::exception& /* e: unused */)
{
}

void TestProgressDialog::onAssertExceptionEndWithUnexpectedException(const std::exception& /* e: unused */)
{
}

void TestProgressDialog::onAssertExceptionEndWithEllipsisException()
{
}

void TestProgressDialog::onAssertNoExceptionEndWithStdException(const std::exception& /* e: unused */)
{
}

void TestProgressDialog::onAssertNoExceptionEndWithEllipsisException()
{
}
