#-------------------------------------------------
#
# Project created by QtCreator 2013-05-23T00:45:15
#
#-------------------------------------------------

include(../proconfig.pri)

QT       += core

QT       -= gui

TARGET = QToyTimeWinLauncher

CONFIG   -= app_bundle

TEMPLATE = app

HEADERS += ../commons/sqhandlegstpath.h

SOURCES += main.cpp \
            ../commons/sqhandlegstpath.cpp \
            ../commons/proj_handle_path.cpp

DESTDIR = $$BASE_PATH

win32 {
        RC_FILE = ../QToyTime/QToyTime.rc
}

