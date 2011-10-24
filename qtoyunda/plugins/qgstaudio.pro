include(../../proconfig.pri)


TEMPLATE = lib

CONFIG += plugin

unix {
    CONFIG += link_pkgconfig
    PKGCONFIG += gstreamer-0.10 QtGStreamer-0.10
}

HEADERS = qgstaudioplayer.h \
          ../fileplayer.h

SOURCES = qgstaudioplayer.cpp \
          ../fileplayer.cpp \
          ../scomponent.cpp \
          ../sqarg.cpp

TARGET = $$qtLibraryTarget(qtoyunda_qgstaudioplayer)
DESTDIR = $$PLUGIN_PATH

PKG_CONFIG +=

win32:LIBS += C:/qt-gstreamer/install/lib/QtGStreamer-0.10.lib \
              C:/qt-gstreamer/install/lib/QtGLib-2.0.lib

win32:INCLUDEPATH += C:/qt-gstreamer/install/include/QtGStreamer/
win32:INCLUDEPATH += C:/boost_1_47_0/
win32:DEPENDPATH += C:/qt-gstreamer/install/include/QtGStreamer/
