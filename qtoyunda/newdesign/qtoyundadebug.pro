TEMPLATE = app

TARGET = qtoyundadebug
INCLUDEPATH += .

HEADERS = debugrenderer.h \
	  toyundarenderer.h \
	  toyundasub.h \
	  toyundasubstream.h \
	  fakeplayer.h \
	  fileplayer.h \
	  rawsubstream.h

SOURCES = debugmain.cpp \
          debugrenderer.cpp \
          toyundarenderer.cpp \
	  toyundasub.cpp \
	  fakeplayer.cpp \
	  rawsubstream.cpp \
	  toyundasubstream.cpp \
	  fileplayer.cpp

