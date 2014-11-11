#-------------------------------------------------
#
# Project created by QtCreator 2014-11-07T13:00:57
#
#-------------------------------------------------

QT       += testlib
QT       -= gui

TARGET = fluent_test
CONFIG   += console
CONFIG   -= app_bundle
CONFIG += c++11

TEMPLATE = app


SOURCES += \
    test/main.cpp \
    test/sendviatcptest.cpp \
    test/testutil.cpp \
    test/externalprocess.cpp \
    test/testrunner.cpp

HEADERS += \
    test/sendviatcptest.h \
    test/testutil.h \
    test/externalprocess.h \
    test/testrunner.h

include(msgpack-c.pri)
include(fluent-logger-qt.pri)
