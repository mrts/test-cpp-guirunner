#include "PluginRunner.h"

#include <pluginloader/pluginloader.h>

#ifdef _WIN32
const char* const LIBNAME = "testprogress.dll";
#else
#include <string>
#include <libgen.h>
const char* const LIBNAME = "libtestprogress.so";
#endif

typedef void (*test_start_ptr)(void *, int, char**);

namespace Test
{

void PluginRunner::start()
{
#ifdef _WIN32
    PluginLoader::PluginLoader pluginLoader(LIBNAME);
#else
    std::string app_path;

    {
        // need to make a copy as dirname() modifies its argument
        std::string app_name(_argv[0]);
        // const_cast<> is bad, but simplifies things a lot here and
        // does no harm as we do not touch app_name afterwards
        app_path = dirname(const_cast<char *>(app_name.c_str()));
    }

    PluginLoader::PluginLoader pluginLoader((app_path + "/" + LIBNAME).c_str());
#endif

    test_start_ptr test_start = reinterpret_cast<test_start_ptr>(
                pluginLoader.getFunction("test_start"));

    test_start(&_controller, _argc, _argv);
}

}
