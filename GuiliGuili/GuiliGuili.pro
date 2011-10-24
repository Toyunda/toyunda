

TARGET = GuiliGuili

HEADERS += \
    songlisttreeitemmodel.h \
    song.h \
    playlistmodel.h \
    playlist.h \
    GuiliGuili.h \
    configdialog.h

SOURCES += \
    songlisttreeitemmodel.cpp \
    song.cpp \
    playlistmodel.cpp \
    playlist.cpp \
    main.cpp \
    GuiliGuili.cpp \
    configdialog.cpp

FORMS += \
    GuiliGuili.ui \
    configDialog.ui

RESOURCES += \
    icone.qrc

DESTDIR = $$DESTDIR_PATH/

DEFINES += PLUGIN_PATH="$$IN_PWD/../qtoyunda/plugins"
