include(../../proconfig.pri)

TEMPLATE = lib
CONFIG += plugin

HEADERS = debugrenderer.h \
          ../toyundasubstream.h

SOURCES = debugrenderer.cpp \
           ../toyundarenderer.cpp \
           ../scomponent.cpp \
        ../sqarg.cpp \
           ../toyundasub.cpp \
           ../toyundasubstream.cpp




TARGET = $$qtLibraryTarget(qtoyunda_debugrenderer)

DESTDIR = $$PLUGIN_PATH
