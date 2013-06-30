include(../proconfig.pri)

TEMPLATE = app

TARGET = qtoyunda-cmd

DESTDIR = $$BASE_PATH

SOURCES += \
    main.cpp \
    qdebugerrorhandler.cpp \
    ../commons/sqarg.cpp


win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../qtoyunda-lib/release/ -lqtoyunda_static
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../qtoyunda-lib/debug/ -lqtoyunda_static
else:symbian: LIBS += -lqtoyunda_static
else:unix: LIBS += -L$$OUT_PWD/../qtoyunda-lib/ -lqtoyunda_static

INCLUDEPATH += ../qtoyunda-lib/ ../commons/

win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../qtoyunda-lib/release/qtoyunda_static.lib
else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../qtoyunda-lib/debug/qtoyunda_static.lib
else:unix:!symbian: PRE_TARGETDEPS += $$OUT_PWD/../qtoyunda-lib/libqtoyunda_static.a

RESOURCES += \
    qtoyunda-cmd.qrc

HEADERS += \
    ../commons/sqerrorhandler.h \
    qdebugerrorhandler.h \
    ../commons/sqarg.h
