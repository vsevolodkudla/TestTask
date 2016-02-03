#-------------------------------------------------
#
# Project created by QtCreator 2016-01-27T17:44:28
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TestTask
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    merger.cpp \
    recorder.cpp

HEADERS  += mainwindow.h \
    merger.h \
    recorder.h

FORMS    += mainwindow.ui

RESOURCES += \
    Files.qrc
