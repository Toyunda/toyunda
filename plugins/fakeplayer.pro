include(../proconfig.pri)

TEMPLATE = lib
CONFIG += plugin

INCLUDEPATH += ../qtoyunda-lib/ ../comons/

HEADERS = fakeplayer.h \
          ../qtoyunda-lib/fileplayer.h

SOURCES = fakeplayer.cpp \
          ../comons/scomponent.cpp \
          ../comons/sqarg.cpp

TARGET = $$qtLibraryTarget(qtoyunda_fakeplayer)
DESTDIR = $$PLUGIN_PATH
