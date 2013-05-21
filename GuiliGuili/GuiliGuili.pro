include(../proconfig.pri)

TARGET = GuiliGuili

HEADERS += \
    song.h \
    playlist.h \
    GuiliGuili.h \
    configdialog.h \
    playlistmodel.h \
    graphicerrorhandler.h \
    ../comons/sqerrorhandler.h \
    profil.h \
    profilosd.h \
    profilmplayer.h \
    mplayerprofildialog.h \
    profilmodel.h \
    osdprofildialog.h \
    profilgsttoyundaplayer.h \
    songtreemodel.h \
    songtreeitem.h \
    gstplayerprofildialog.h \
    ../comons/sqhandlegstpath.h


SOURCES += \
    song.cpp \
    playlist.cpp \
    main.cpp \
    GuiliGuili.cpp \
    configdialog.cpp \
    playlistmodel.cpp \
    graphicerrorhandler.cpp \
    profil.cpp \
    profilosd.cpp \
    profilmplayer.cpp \
    mplayerprofildialog.cpp \
    profilmodel.cpp \
    osdprofildialog.cpp \
    profilgsttoyundaplayer.cpp \
    songtreemodel.cpp \
    songtreeitem.cpp \
    gstplayerprofildialog.cpp \
    ../comons/sqhandlegstpath.cpp

FORMS += \
    GuiliGuili.ui \
    configDialog.ui \
    osdprofildialog.ui \
    mplayerprofildialog.ui \
    gstplayerprofildialog.ui

RESOURCES += \
    icone.qrc

DESTDIR = $$BASE_PATH

win32 {
	RC_FILE = GuiliGuili.rc
}

message($$BASE_PATH)

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../qtoyunda-lib/release/ -lqtoyunda_static
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../qtoyunda-lib/debug/ -lqtoyunda_static
else:symbian: LIBS += -lqtoyunda_static
else:unix: LIBS += -L$$OUT_PWD/../qtoyunda-lib/ -lqtoyunda_static

INCLUDEPATH += ../comons/
INCLUDEPATH += $$PWD/../qtoyunda-lib/
DEPENDPATH += $$PWD/../qtoyunda-lib/


win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../qtoyunda-lib/release/qtoyunda_static.lib
else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../qtoyunda-lib/debug/qtoyunda_static.lib
else:unix:!symbian: PRE_TARGETDEPS += $$OUT_PWD/../qtoyunda-lib/libqtoyunda_static.a




