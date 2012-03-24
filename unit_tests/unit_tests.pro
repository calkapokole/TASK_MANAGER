#-------------------------------------------------
#
# Project created by QtCreator 2012-03-24T01:50:20
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = tst_TaskManagerTest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += tst_TaskManagerTest.cpp \
    ../TaskSet.cpp \
    ../TaskManager.cpp \
    ../Task.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\" \
    QT_USE_QSTRINGBUILDER

HEADERS += \
    ../TaskSet.h \
    ../TaskManager.h \
    ../Task.h
