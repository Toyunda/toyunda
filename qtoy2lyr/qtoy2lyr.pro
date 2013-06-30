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

SOURCES += main.cpp\
        qtoy2lyr.cpp\
        ../QToyTime/toyundagendialog.cpp \
        ../QToyTime/lyrsyntaxhighlighter.cpp \
        ../QToyTime/frmsyntaxhighlighter.cpp

HEADERS  += qtoy2lyr.h \
            ../QToyTime/toyundagendialog.h \
            ../QToyTime/lyrsyntaxhighlighter.h \
            ../QToyTime/frmsyntaxhighlighter.h

INCLUDEPATH += ../QToyTime

FORMS    += qtoy2lyr.ui \
            ../QToyTime/toyundagendialog.ui
