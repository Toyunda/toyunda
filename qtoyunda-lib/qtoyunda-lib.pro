TEMPLATE = lib

CONFIG += staticlib

TARGET = qtoyunda_static


HEADERS += \
    toyundasubstream.h \
    toyundasub.hxx \
    toyundasub.h \
    toyundarenderer.h \
    fileplayer.h \
    ../comons/sqarg.h \
    ../comons/scomponent.h \
    ../comons/sqerror.h  \
    ../comons/sqerrorhandler.h \
    rawsubstream.h \
    qtoyunda.h \
    abstractfileplayer.h \
    abstractrenderer.h

SOURCES += \
    toyundasubstream.cpp \
    toyundasub.cpp \
    toyundarenderer.cpp \
    ../comons/sqarg.cpp \
    ../comons/scomponent.cpp \
    ../comons/sqerror.cpp \
    rawsubstream.cpp \
    qtoyunda.cpp

INCLUDEPATH += ../comons/



