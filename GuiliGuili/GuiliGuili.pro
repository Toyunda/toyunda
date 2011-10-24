include(../proconfig.pri)

TARGET = GuiliGuili

HEADERS += \
    songlisttreeitemmodel.h \
    song.h \
    playlist.h \
    GuiliGuili.h \
    configdialog.h \
    playlistmodel.h


SOURCES += \
    songlisttreeitemmodel.cpp \
    song.cpp \
    playlist.cpp \
    main.cpp \
    GuiliGuili.cpp \
    configdialog.cpp \
    playlistmodel.cpp

FORMS += \
    GuiliGuili.ui \
    configDialog.ui

RESOURCES += \
    icone.qrc

DESTDIR = $$BASE_PATH

message($$BASE_PATH)

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../qtoyunda/release/ -lqtoyunda_static
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../qtoyunda/debug/ -lqtoyunda_static
else:symbian: LIBS += -lqtoyunda_static
else:unix: LIBS += -L$$OUT_PWD/../qtoyunda -lqtoyunda_static

INCLUDEPATH += $$PWD/../qtoyunda
DEPENDPATH += $$PWD/../qtoyunda

win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../qtoyunda/release/qtoyunda_static.lib
else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../qtoyunda/debug/qtoyunda_static.lib
else:unix:!symbian: PRE_TARGETDEPS += $$OUT_PWD/../qtoyunda/libqtoyunda_static.a
