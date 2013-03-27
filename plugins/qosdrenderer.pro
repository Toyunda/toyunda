include(../proconfig.pri)

QT += gui
TEMPLATE = lib
CONFIG += plugin

unix {
    CONFIG += link_pkgconfig
    PKGCONFIG += xfixes
}

INCLUDEPATH += ../qtoyunda-lib ../comons/

HEADERS = qosd.h \
          ../qtoyunda-lib/toyundasubstream.h

SOURCES = qosd.cpp \
           ../qtoyunda-lib/toyundarenderer.cpp \
           ../comons/scomponent.cpp \
           ../comons/sqarg.cpp \
           ../comons/sqerror.cpp \
           ../qtoyunda-lib/toyundasub.cpp \
           ../qtoyunda-lib/toyundasubstream.cpp \
           ../comons/toyundadrawer.cpp

TARGET = $$qtLibraryTarget(qtoyunda_qosdrenderer)
DESTDIR = $$PLUGIN_PATH
