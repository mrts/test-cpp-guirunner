#include <QtGui/QApplication>

#include "TestProgressDialog.h"

extern "C" Q_DECL_EXPORT
void test_start(void *testController, int argc, char** argv)
{
    QApplication app(argc, argv);

    TestProgressDialog progressDialog(testController);
    progressDialog.show();

    app.exec();
}
