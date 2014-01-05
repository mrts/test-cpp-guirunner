#ifndef TESTPLUGINRUNNER_H__
#define TESTPLUGINRUNNER_H__

namespace Test
{

class Controller;

class PluginRunner
{
public:
    PluginRunner(Controller& controller, int argc, char** argv) :
        _controller(controller),
        _argc(argc),
        _argv(argv)
    { }

    void start();

private:
    PluginRunner(const PluginRunner&);
    PluginRunner& operator=(const PluginRunner&);

    Controller& _controller;
    int _argc;
    char** _argv;
};

}

#endif /* TESTPLUGINRUNNER_H */
