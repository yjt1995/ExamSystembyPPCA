#-------------------------------------------------
#
# Project created by QtCreator 2014-07-24T08:34:17
#
#-------------------------------------------------

QT       += core gui network
LIBS += -lwsock32
LIBS += -lws2_32

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = client-ceshi1
TEMPLATE = app


SOURCES += main.cpp\
        dialog.cpp \
    messagedialog.cpp \
    spifirewall.cpp \
    stuserver.cpp \
    stuserverthread.cpp \
    screenwindow.cpp

HEADERS  += dialog.h \
    messagedialog.h \
    spifirewall.h \
    stuserver.h \
    stuserverthread.h \
    screenwindow.h

FORMS    += dialog.ui \
    messagedialog.ui \
    screenwindow.ui

RESOURCES += \
    clientico.qrc
