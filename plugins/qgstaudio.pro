include(../proconfig.pri)


TEMPLATE = lib

CONFIG += plugin

QT += widgets multimedia

INCLUDEPATH += ../qtoyunda-lib ../commons/

unix {
    CONFIG += link_pkgconfig
    PKGCONFIG += gstreamer-1.0 QtGStreamer-1.0
}

OTHER_FILES += qgstaudio.json

HEADERS = qgstaudioplayer.h \
          ../qtoyunda-lib/fileplayer.h

SOURCES = qgstaudioplayer.cpp \
          ../commons/scomponent.cpp \
          ../commons/sqarg.cpp \
          ../commons/sqerror.cpp

TARGET = $$qtLibraryTarget(qtoyunda_qgstaudioplayer)
DESTDIR = $$PLUGIN_PATH


win32:LIBS += C:/qt-gstreamer/install/lib/Qt5GStreamer-1.0.lib \
              C:/qt-gstreamer/install/lib/Qt5GLib-2.0.lib

win32:INCLUDEPATH += C:/qt-gstreamer/install/include/Qt5GStreamer/
win32:INCLUDEPATH += C:/boost_1_59_0/
win32:DEPENDPATH += C:/qt-gstreamer/install/include/Qt5GStreamer/

DISTFILES += \
    qgstaudio.json
