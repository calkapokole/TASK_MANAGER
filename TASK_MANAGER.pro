#-------------------------------------------------
#
# Project created by QtCreator 2012-03-18T16:00:38
#
#-------------------------------------------------

QT       += core
QT       -= gui

DEFINES *= QT_USE_QSTRINGBUILDER

TARGET = TASK_MANAGER
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    Task.cpp \
    TaskManager.cpp \
    TaskSet.cpp

HEADERS += \
    Task.h \
    TaskManager.h \
    TaskSet.h \
    EmptyStringException.h \
    InvalidEnumException.h \
    InvalidDataTypeException.h
