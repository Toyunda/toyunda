#-------------------------------------------------
#
# Project created by QtCreator 2013-05-06T16:43:09
#
#-------------------------------------------------

include(../proconfig.pri)

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

unix {
    CONFIG += link_pkgconfig
    PKGCONFIG += gstreamer-0.10 QtGStreamer-0.10 QtGStreamerUi-0.10
}

TARGET = QToyTime
TEMPLATE = app


SOURCES += main.cpp\
        qtoytime.cpp \
    videowidget.cpp \
    newtimedialog.cpp \
    generatetoyundasubtite.cpp \
    lyrsyntaxhighlighter.cpp \
    frmsyntaxhighlighter.cpp \
    ../comons/sqhandlegstpath.cpp \
    previewwindow.cpp

HEADERS  += qtoytime.h \
    videowidget.h \
    newtimedialog.h \
    generatetoyundasubtitle.h \
    lyrsyntaxhighlighter.h \
    frmsyntaxhighlighter.h  \
    ../comons/sqhandlegstpath.h \
    previewwindow.h


DESTDIR = $$BASE_PATH

FORMS    += qtoytime.ui \
    newtimedialog.ui

win32:LIBS += C:/qt-gstreamer/install/lib/QtGStreamer-0.10.lib \
              C:/qt-gstreamer/install/lib/QtGLib-2.0.lib \
              C:/qt-gstreamer/install/lib/QtGStreamerUi-0.10.lib

win32:INCLUDEPATH += C:/qt-gstreamer/install/include/QtGStreamer/
win32:INCLUDEPATH += C:/boost_1_53_0/boost_1_53_0/
win32:DEPENDPATH += C:/qt-gstreamer/install/include/QtGStreamer/

