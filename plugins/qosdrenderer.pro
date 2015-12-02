include(../proconfig.pri)

QT += gui widgets
TEMPLATE = lib
CONFIG += plugin

unix {
    CONFIG += link_pkgconfig
    PKGCONFIG += xfixes
}

INCLUDEPATH += ../qtoyunda-lib ../commons/

HEADERS = qosd.h \
          ../qtoyunda-lib/toyundasubstream.h

OTHER_FILES += qosdrenderer.json

SOURCES = qosd.cpp \
           ../qtoyunda-lib/toyundarenderer.cpp \
           ../commons/scomponent.cpp \
           ../commons/sqarg.cpp \
           ../commons/sqerror.cpp \
           ../qtoyunda-lib/toyundasub.cpp \
           ../qtoyunda-lib/toyundasubstream.cpp \
           ../commons/toyundadrawer.cpp

TARGET = $$qtLibraryTarget(qtoyunda_qosdrenderer)
DESTDIR = $$PLUGIN_PATH

DISTFILES += \
    qosdrenderer.json
