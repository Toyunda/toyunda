TEMPLATE = lib

CONFIG += staticlib

TARGET = qtoyunda_static

DESTDIR_TARGET = ./

HEADERS += \
    toyundasubstream.h \
    toyundasub.hxx \
    toyundasub.h \
    toyundarenderer.h \
    toyundadrawer.h \
    sqarg.h \
    scomponent.h \
    rawtoyundastream.h \
    rawsubstream.h \
    qtoyunda.h \
    abstractfileplayer.h

SOURCES += \
    toyundasubstream.cpp \
    toyundasub.cpp \
    toyundarenderer.cpp \
    toyundadrawer.cpp \
    sqarg.cpp \
    scomponent.cpp \
    rawsubstream.cpp \
    qtoyunda.cpp


