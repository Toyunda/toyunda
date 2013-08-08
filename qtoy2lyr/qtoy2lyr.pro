#-------------------------------------------------
#
# Project created by QtCreator 2013-06-26T23:08:02
#
#-------------------------------------------------

include (../proconfig.pri)

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qtoy2lyr
TEMPLATE = app

DESTDIR = $$BASE_PATH

SOURCES +=\
        qtoy2lyr.cpp\
        ../QToyTime/toyundagendialog.cpp \
        ../QToyTime/lyrsyntaxhighlighter.cpp \
        ../QToyTime/frmsyntaxhighlighter.cpp \
        ../commons/sqhandlegstpath.cpp \
        maintoy2lyr.cpp \
        ../commons/proj_handle_path.cpp

HEADERS  += qtoy2lyr.h \
            ../QToyTime/toyundagendialog.h \
            ../QToyTime/lyrsyntaxhighlighter.h \
            ../QToyTime/frmsyntaxhighlighter.h \
            ../commons/sqhandlegstpath.h \
            ../commons/proj_handle_path.h \
            ../commons/proj_config.h

INCLUDEPATH += ../QToyTime \
               ../commons

FORMS    += qtoy2lyr.ui \
            ../QToyTime/toyundagendialog.ui
