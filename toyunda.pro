include(proconfig.pri)


CONFIG += ordered

TEMPLATE = subdirs

SUBDIRS += \
	qtoyunda-lib \
	qtoyunda-cmd  \
	plugins	\
	GuiliGuili \
        QToyTime \
        qttoyunda-player \
        qtoy2lyr

win32 {
        SUBDIRS += QToyTimeWinLauncher
}

QT += widgets

