include(../proconfig.pri)

TEMPLATE = lib
CONFIG += plugin

INCLUDEPATH += ../qtoyunda-lib/ ../commons/

HEADERS = fakeplayer.h \
          ../qtoyunda-lib/fileplayer.h

OTHER_FILES += fakeplayer.json

SOURCES = fakeplayer.cpp \
          ../commons/scomponent.cpp \
          ../commons/sqarg.cpp \
          ../commons/sqerror.h \
          ../commons/sqerror.cpp

TARGET = $$qtLibraryTarget(qtoyunda_fakeplayer)
DESTDIR = $$PLUGIN_PATH

DISTFILES += \
    fakeplayer.json
