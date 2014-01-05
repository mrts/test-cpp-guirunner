#include <testcpp/testcpp.h>
#include <PluginRunner.h>

#include <iostream>

using namespace std;

class Suite1 : public Test::Suite
{
public:
    void test()
    {
        Test::assertEqual("1 == 1", 1, 1);
    }
};

class Suite2 : public Test::Suite
{
    void test()
    {
        Test::assertEqual("2 == 3", 2, 3);
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
