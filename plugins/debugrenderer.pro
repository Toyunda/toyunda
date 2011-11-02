include(../proconfig.pri)

TEMPLATE = lib
CONFIG += plugin

HEADERS = debugrenderer.h \
          ../qtoyunda-lib/toyundasubstream.h

SOURCES = debugrenderer.cpp \
           ../qtoyunda-lib/toyundarenderer.cpp \
           ../comons/scomponent.cpp \
        ../comons/sqarg.cpp \
           ../qtoyunda-lib/toyundasub.cpp \
           ../qtoyunda-lib/toyundasubstream.cpp


INCLUDEPATH += ../qtoyunda-lib/ ../comons/

TARGET = $$qtLibraryTarget(qtoyunda_debugrenderer)

DESTDIR = $$PLUGIN_PATH
