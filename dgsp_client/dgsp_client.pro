QT += core
QT -= gui
QT += testlib network widgets

CONFIG += c++11

TARGET = dgsp_client
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    tcphandler.cpp \
    inputparser.cpp \
    inputvalidator.cpp \
    payloadsender.cpp \
    networkclient.cpp \
    guiclient.cpp \
    gamecontrol.cpp \
    gamestats.cpp \
    connectionstate.cpp \
    userstate.cpp \
    repocache.cpp \
    messagestate.cpp \
    gamedisplay.cpp \
    gamemanager.cpp \
    sendhandler.cpp \
    shapecontrol.cpp \
    gamekey.cpp

HEADERS += \
    tcphandler.h \
    inputparser.h \
    inputvalidator.h \
    payloadsender.h \
    globalvalues.h \
    networkclient.h \
    guiclient.h \
    gamecontrol.h \
    gamestats.h \
    connectionstate.h \
    userstate.h \
    repocache.h \
    messagestate.h \
    gamedisplay.h \
    gamemanager.h \
    sendhandler.h \
    shapecontrol.h \
    gamekey.h
