QT += core
QT -= gui
QT += testlib network widgets

CONFIG += c++11

TARGET = dgsp_client
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    client.cpp

HEADERS += \
    client.h
