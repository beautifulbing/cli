QT += core network
QT -= gui

CONFIG += c++11

TARGET = bbcli
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    beautifulbing.cpp \
    bingextractor.cpp \
    imagemeta.cpp \
    wallpaper.cpp

HEADERS += \
    beautifulbing.h \
    bingextractor.h \
    imagemeta.h \
    wallpaper.h
