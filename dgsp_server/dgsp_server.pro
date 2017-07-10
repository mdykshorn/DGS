QT += core
QT -= gui
QT += testlib network widgets

CONFIG += c++11

TARGET = server
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    dialog.cpp \
    gameserver.cpp \
    serverthread.cpp \
    inputparser.cpp \
    inputvalidator.cpp \
    payloadsender.cpp \
    repositorymanager.cpp \
    sendstate.cpp \
    tcphandler.cpp

DISTFILES +=

HEADERS += \
    dialog.h \
    serverthread.h \
    gameserver.h \
    inputparser.h \
    inputvalidator.h \
    payloadsender.h \
    repositorymanager.h \
    globalvalues.h \
    sendstate.h \
    tcphandler.h
