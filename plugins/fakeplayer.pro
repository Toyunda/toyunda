include(../proconfig.pri)

TEMPLATE = lib
CONFIG += plugin

INCLUDEPATH += ../qtoyunda-lib/ ../commons/

HEADERS = fakeplayer.h \
          ../qtoyunda-lib/fileplayer.h

SOURCES = fakeplayer.cpp \
          ../commons/scomponent.cpp \
          ../commons/sqarg.cpp

TARGET = $$qtLibraryTarget(qtoyunda_fakeplayer)
DESTDIR = $$PLUGIN_PATH
