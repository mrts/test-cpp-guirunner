TARGET   = test
TEMPLATE = app

CONFIG  -= qt
CONFIG  += warn_on console

CONFIG(release, debug|release): BUILDMODE = release
CONFIG(debug,   debug|release): BUILDMODE = debug

DESTDIR     = bin/$$BUILDMODE
OBJECTS_DIR = obj/$$BUILDMODE

HEADERS += src/do_sleep.h
SOURCES += src/main.cpp

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
