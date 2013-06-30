TEMPLATE = lib

CONFIG += staticlib

TARGET = qtoyunda_static


HEADERS += \
    toyundasubstream.h \
    toyundasub.hxx \
    toyundasub.h \
    toyundarenderer.h \
    fileplayer.h \
    ../commons/sqarg.h \
    ../commons/scomponent.h \
    ../commons/sqerror.h  \
    ../commons/sqerrorhandler.h \
    rawsubstream.h \
    qtoyunda.h \
    abstractfileplayer.h \
    abstractrenderer.h

SOURCES += \
    toyundasubstream.cpp \
    toyundasub.cpp \
    toyundarenderer.cpp \
    ../commons/sqarg.cpp \
    ../commons/scomponent.cpp \
    ../commons/sqerror.cpp \
    rawsubstream.cpp \
    qtoyunda.cpp

INCLUDEPATH += ../commons/



