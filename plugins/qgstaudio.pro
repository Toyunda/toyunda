include(../proconfig.pri)


TEMPLATE = lib

CONFIG += plugin

INCLUDEPATH += ../qtoyunda-lib ../comons/

unix {
    CONFIG += link_pkgconfig
    PKGCONFIG += gstreamer-0.10 QtGStreamer-0.10
}

HEADERS = qgstaudioplayer.h \
          ../qtoyunda-lib/fileplayer.h

SOURCES = qgstaudioplayer.cpp \
          ../comons/scomponent.cpp \
          ../comons/sqarg.cpp

TARGET = $$qtLibraryTarget(qtoyunda_qgstaudioplayer)
DESTDIR = $$PLUGIN_PATH


win32:LIBS += C:/qt-gstreamer/install/lib/QtGStreamer-0.10.lib \
              C:/qt-gstreamer/install/lib/QtGLib-2.0.lib

win32:INCLUDEPATH += C:/qt-gstreamer/install/include/QtGStreamer/
win32:INCLUDEPATH += C:/boost_1_47_0/
win32:DEPENDPATH += C:/qt-gstreamer/install/include/QtGStreamer/
