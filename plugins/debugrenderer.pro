include(../proconfig.pri)

TEMPLATE = lib
CONFIG += plugin

HEADERS = debugrenderer.h \
          ../qtoyunda-lib/toyundasubstream.h

SOURCES = debugrenderer.cpp \
           ../qtoyunda-lib/toyundarenderer.cpp \
           ../commons/scomponent.cpp \
        ../commons/sqarg.cpp \
           ../qtoyunda-lib/toyundasub.cpp \
           ../qtoyunda-lib/toyundasubstream.cpp


INCLUDEPATH += ../qtoyunda-lib/ ../commons/

TARGET = $$qtLibraryTarget(qtoyunda_debugrenderer)

DESTDIR = $$PLUGIN_PATH
