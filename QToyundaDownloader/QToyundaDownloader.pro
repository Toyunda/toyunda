#-------------------------------------------------
#
# Project created by QtCreator 2013-05-24T11:54:18
#
#-------------------------------------------------

QT       += core gui network
CONFIG  += static
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QToyundaDownloader
TEMPLATE = app


SOURCES += main.cpp\
        qtoyundadownloader.cpp

HEADERS  += qtoyundadownloader.h

FORMS    += qtoyundadownloader.ui

OTHER_FILES += \
    configDL.ini

RESOURCES += \
    qtoyundadownloader.qrc
