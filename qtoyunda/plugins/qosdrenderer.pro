include(../../proconfig.pri)

QT += Gui
TEMPLATE = lib
CONFIG += plugin

INCLUDEPATH += ../

HEADERS = qosd.h \
          ../toyundasubstream.h

SOURCES = qosd.cpp \
           ../toyundarenderer.cpp \
           ../scomponent.cpp \
           ../sqarg.cpp \
           ../toyundasub.cpp \
           ../toyundasubstream.cpp \
           ../toyundadrawer.cpp

TARGET = $$qtLibraryTarget(qtoyunda_qosdrenderer)
DESTDIR = $$PLUGIN_PATH
