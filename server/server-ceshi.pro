#-------------------------------------------------
#
# Project created by QtCreator 2014-08-04T16:28:46
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = server-ceshi
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    messagedialog.cpp \
    teaclientthread.cpp

HEADERS  += mainwindow.h \
    messagedialog.h \
    teaclientthread.h

FORMS    += mainwindow.ui \
    messagedialog.ui

RESOURCES += \
    serverico.qrc
