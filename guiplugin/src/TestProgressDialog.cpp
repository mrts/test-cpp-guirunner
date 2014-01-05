#include "TestProgressDialog.h"
#include "ui_TestProgressDialog.h"

#include "TestRunnerThread.h"

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

    connect(testRunnerThread, SIGNAL(finished()),
           this, SLOT(onTestRunnerFinished()));
    connect(testRunnerThread, SIGNAL(terminated()),
           this, SLOT(onTestRunnerTerminated()));

    connect(testRunnerThread, SIGNAL(allTestSuitesBegin(int)),
           this, SLOT(onAllTestSuitesBegin(int)));
    connect(testRunnerThread, SIGNAL(allTestSuitesEnd(int, int, int, int)),
           this, SLOT(onAllTestSuitesEnd(int, int, int, int)));

    connect(testRunnerThread, SIGNAL(testSuiteBegin(const std::string&, int, int)),
           this, SLOT(onTestSuiteBegin(const std::string&, int, int)));

    connect(testRunnerThread, SIGNAL(testSuiteEnd(int)),
           this, SLOT(onTestSuiteEnd(int)));
    connect(testRunnerThread, SIGNAL(testSuiteEndWithStdException(int, const std::exception&)),
           this, SLOT(onTestSuiteEndWithStdException(int, const std::exception&)));
    connect(testRunnerThread, SIGNAL(testSuiteEndWithEllipsisException(int)),
           this, SLOT(onTestSuiteEndWithEllipsisException(int)));

    connect(testRunnerThread, SIGNAL(assertBegin(const std::string&)),
           this, SLOT(onAssertBegin(const std::string&)));
    connect(testRunnerThread, SIGNAL(assertExceptionBegin(const std::string&)),
           this, SLOT(onAssertExceptionBegin(const std::string&)));
    connect(testRunnerThread, SIGNAL(assertNoExceptionBegin(const std::string&)),
           this, SLOT(onAssertNoExceptionBegin(const std::string&)));

    connect(testRunnerThread, SIGNAL(assertEnd(bool)),
           this, SLOT(onAssertEnd(bool)));
    connect(testRunnerThread, SIGNAL(assertExceptionEndWithExpectedException(const std::exception&)),
           this, SLOT(onAssertExceptionEndWithExpectedException(const std::exception&)));
    connect(testRunnerThread, SIGNAL(assertExceptionEndWithUnexpectedException(const std::exception&)),
           this, SLOT(onAssertExceptionEndWithUnexpectedException(const std::exception&)));
    connect(testRunnerThread, SIGNAL(assertExceptionEndWithEllipsisException()),
           this, SLOT(onAssertExceptionEndWithEllipsisException()));
    connect(testRunnerThread, SIGNAL(assertNoExceptionEndWithStdException(const std::exception&)),
           this, SLOT(onAssertNoExceptionEndWithStdException(const std::exception&)));
    connect(testRunnerThread, SIGNAL(assertNoExceptionEndWithEllipsisException()),
           this, SLOT(onAssertNoExceptionEndWithEllipsisException()));

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
}

void TestProgressDialog::onCancel()
{
    // cancel testRunnerThread:
    // if (cancelRequested)
    //    throw exception that is not caught by Controller
    // wrap controller.run() in try-catch that will catch it
    //
    // testRunnerThread->wait();
    // onTestRunnerFinished();
}

void TestProgressDialog::onTestRunnerFinished()
{
    ui->okButton->setEnabled(true);
    ui->cancelButton->setHidden(true);
}

void TestProgressDialog::onTestRunnerTerminated()
{
    onTestRunnerFinished();
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

void TestProgressDialog::onAssertNoExceptionEndWithStdException(const std::exception& e)
{
}

void TestProgressDialog::onAssertNoExceptionEndWithEllipsisException()
{
}
