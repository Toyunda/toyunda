include(../proconfig.pri)

QT += gui
TEMPLATE = lib
CONFIG += plugin

INCLUDEPATH += ../qtoyunda-lib ../comons/

HEADERS = qosd.h \
          ../qtoyunda-lib/toyundasubstream.h

SOURCES = qosd.cpp \
           ../qtoyunda-lib/toyundarenderer.cpp \
           ../comons/scomponent.cpp \
           ../comons/sqarg.cpp \
           ../qtoyunda-lib/toyundasub.cpp \
           ../qtoyunda-lib/toyundasubstream.cpp \
           ../comons/toyundadrawer.cpp

TARGET = $$qtLibraryTarget(qtoyunda_qosdrenderer)
DESTDIR = $$PLUGIN_PATH
