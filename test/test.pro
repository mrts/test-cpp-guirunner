TARGET   = test
TEMPLATE = app

CONFIG  -= qt
CONFIG  += warn_on

CONFIG(release, debug|release): BUILDMODE = release
CONFIG(debug,   debug|release): BUILDMODE = debug

DESTDIR     = bin/$$BUILDMODE
OBJECTS_DIR = obj/$$BUILDMODE

SOURCES += src/*.cpp

PLUGINRUNNERLIB = ../pluginrunner
INCLUDEPATH    += $$PLUGINRUNNERLIB/include
LIBS           += -L$$PLUGINRUNNERLIB/lib/$$BUILDMODE -lpluginrunner \
                  -L../lib/pluginloader-cpp/lib -lpluginloader

TESTLIB         = ../lib/test-cpp
INCLUDEPATH    += $$TESTLIB/include
LIBS           += -L$$TESTLIB/lib -ltestcpp

unix {
LIBS           += -ldl
}
