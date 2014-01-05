TARGET   = testprogress
TEMPLATE = lib

CONFIG  += warn_on dll
QT      += core gui

# QMAKE_CXXFLAGS += -std=c++11

CONFIG(release, debug|release): BUILDMODE = release
CONFIG(debug,   debug|release): BUILDMODE = debug

# put library next to binary
DESTDIR     = ../test/bin/$$BUILDMODE
OBJECTS_DIR = obj/$$BUILDMODE
MOC_DIR     = $$OBJECTS_DIR
RCC_DIR     = $$OBJECTS_DIR
UI_DIR      = $$OBJECTS_DIR

SOURCES += src/*.cpp
HEADERS += src/*.h
FORMS   += src/*.ui
