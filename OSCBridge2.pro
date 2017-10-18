#-------------------------------------------------
#
# Project created by QtCreator 2017-10-13T12:00:10
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = OSCBridge2
TEMPLATE = app

VERSION = 2.0.0
QMAKE_TARGET_COMPANY = Raghav Narula
QMAKE_TARGET_PRODUCT = OSC Bridge
QMAKE_TARGET_DESCRIPTION = Bridge OSC to TCP devices
QMAKE_TARGET_COPYRIGHT = Raghav Narula
# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    ioservicepool.cpp \
    tcpclient.cpp \
    tcpsocketwriter.cpp \
    tcpsocketreader.cpp \
    tcpclientresolver.cpp \
    udpserver.cpp \
    udpserversocketreader.cpp \
    oscpacketdecoder.cpp \
    messagehandler.cpp \
    oscmessagedecoder.cpp \
    deviceinteractor.cpp \
    adddevicedialog.cpp \
    qtdevicepresenter.cpp \
    qtdeviceinteractor.cpp \
    deviceconfiguration.cpp \
    deviceconfigurationstore.cpp \
    qtsettingsdeviceconfigurationstore.cpp \
    serverinteractor.cpp \
    client.cpp \
    qtserverinteractor.cpp \
    qtserverpresenter.cpp

HEADERS += \
        mainwindow.h \
    ioservicepool.h \
    tcpclient.h \
    tcpsocketwriter.h \
    tcpsocketreader.h \
    tcpclientresolver.h \
    udpserver.h \
    udpserversocketreader.h \
    oscpacketdecoder.h \
    messagehandler.h \
    oscpkt.hh \
    oscmessagedecoder.h \
    devicestate.h \
    deviceinteractor.h \
    adddevicedialog.h \
    qtdevicepresenter.h \
    qtdeviceinteractor.h \
    deviceconfiguration.h \
    deviceconfigurationstore.h \
    qtsettingsdeviceconfigurationstore.h \
    serverinteractor.h \
    client.h \
    clientstatechangelistener.h \
    statebearer.h \
    statechangelistener.h \
    clientstate.h \
    qtserverinteractor.h \
    qtserverpresenter.h

FORMS += \
        mainwindow.ui \
    adddevicedialog.ui

QMAKE_CXXFLAGS = -std=c++11 -stdlib=libc++
QMAKE_LFLAGS = -std=c++11 -stdlib=libc++

CONFIG += c++11
QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.12


INCLUDEPATH += /usr/local/include

INCLUDEPATH += $$PWD/../logger

LIBS += /usr/local/opt/boost/lib/libboost_system.a

LIBS += /usr/local/opt/boost/lib/libboost_filesystem.a

LIBS += /usr/local/opt/boost/lib/libboost_thread-mt.a
