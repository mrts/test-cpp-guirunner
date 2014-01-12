#include <testcpp/testcpp.h>
#include <PluginRunner.h>

#include <iostream>

#include "do_sleep.h"

using namespace std;

class Suite1 : public Test::Suite
{
public:
    void test()
    {
        assertEqual("1 == 1", 1, 1);
        do_sleep(1);
        assertEqual("2 == 2", 2, 2);
        do_sleep(1);
        assertEqual("1 == 1", 3, 3);
        do_sleep(1);
    }
};

class Suite2 : public Test::Suite
{
    void test()
    {
        assertEqual("2 == 3", 2, 3);
        do_sleep(1);
        assertEqual("4 == 6", 4, 6);
    }
};

int main(int argc, char** argv)
{
    try
    {
        Test::Controller &controller = Test::Controller::instance();
        controller.addTestSuite("Suite1", Test::Suite::instance<Suite1>);
        controller.addTestSuite("Suite2", Test::Suite::instance<Suite2>);

        Test::PluginRunner runner(controller, argc, argv);
        runner.start();

    } catch (const std::exception& e) {
        cout << "FAIL: " << e.what() << endl;
        return -1;
    }

    return 0;
}
