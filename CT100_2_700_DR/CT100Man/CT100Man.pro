#-------------------------------------------------
#
# Project created by QtCreator 2018-03-08T10:12:29
#
#-------------------------------------------------

QT       += core \
        network

QT       -= gui

TARGET = CT100Man
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    usbUpdate/usbUpdate.cpp \
    runApp/runApp.cpp \
    qtSocketServer/SocketConnect.cpp \
    qtSocketServer/TcpSocketServer.cpp

HEADERS += \
    usbUpdate/usbUpdate.h \
    runApp/runApp.h \
    qtSocketServer/SocketConnet.h \
    qtSocketServer/TcpSocketServer.h
