
TEMPLATE = lib
CONFIG += plugin

INCLUDEPATH += ../

HEADERS = fakeplayer.h \
          ../fileplayer.h

SOURCES = fakeplayer.cpp \
          ../fileplayer.cpp \
          ../scomponent.cpp \
          ../sqarg.cpp

TARGET = $$qtLibraryTarget(qtoyunda_fakeplayer)
