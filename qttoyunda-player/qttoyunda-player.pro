#-------------------------------------------------
#
# Project created by QtCreator 2013-06-11T18:19:29
#
#-------------------------------------------------

include(../proconfig.pri)

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qttoyunda-player
TEMPLATE = app

unix {
    CONFIG += link_pkgconfig
    PKGCONFIG += gstreamer-1.0 QtGStreamer-1.0 QtGStreamerUi-1.0
}

SOURCES += main.cpp\
        videoplayer.cpp \
        ../commons/sqhandlegstpath.cpp \
        ../commons/sqarg.cpp \
        ../commons/proj_handle_path.cpp

HEADERS  += videoplayer.h \
            ../commons/proj_handle_path.h \
            ../commons/proj_config.h

DESTDIR = $$BASE_PATH

win32:LIBS += C:/qt-gstreamer/install/lib/Qt5GStreamer-1.0.lib \
              C:/qt-gstreamer/install/lib/Qt5GLib-2.0.lib \
              C:/qt-gstreamer/install/lib/Qt5GStreamerUi-1.0.lib

win32:INCLUDEPATH += C:/qt-gstreamer/install/include/Qt5GStreamer/
win32:INCLUDEPATH += C:/boost_1_59_0/
win32:DEPENDPATH += C:/qt-gstreamer/install/include/Qt5GStreamer/

INCLUDEPATH += ../commons/

