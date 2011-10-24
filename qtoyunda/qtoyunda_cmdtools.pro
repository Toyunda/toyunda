include(../proconfig.pri)

TEMPLATE = app

TARGET = qtoyunda-cmd
DESTDIR = $$BASE_PATH

SOURCES += \
    main.cpp


win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/release/ -lqtoyunda_static
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/debug/ -lqtoyunda_static
else:symbian: LIBS += -lqtoyunda_static
else:unix: LIBS += -L$$OUT_PWD/ -lqtoyunda_static

INCLUDEPATH += $$PWD/
DEPENDPATH += $$PWD/

win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/release/qtoyunda_static.lib
else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/debug/qtoyunda_static.lib
else:unix:!symbian: PRE_TARGETDEPS += $$OUT_PWD/libqtoyunda_static.a
