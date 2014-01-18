# Depends on pluginloader library.
#
# Use the following in client projects:
#  PLUGINRUNNERLIB = ../pluginrunner
#  INCLUDEPATH    += $$PLUGINRUNNERLIB/include
#  LIBS           += -L$$PLUGINRUNNERLIB/lib/$$BUILDMODE -lpluginrunner

TARGET   = pluginrunner
TEMPLATE = lib

CONFIG  -= qt
CONFIG  += warn_on staticlib

CONFIG(release, debug|release): BUILDMODE = release
CONFIG(debug,   debug|release): BUILDMODE = debug

DESTDIR     = lib/$$BUILDMODE
OBJECTS_DIR = obj/$$BUILDMODE

INCLUDEPATH += include \
               ../lib/pluginloader-cpp/include

SOURCES += src/PluginRunner.cpp
HEADERS += include/PluginRunner.h
