include(proconfig.pri)


CONFIG += ordered

TEMPLATE = subdirs

SUBDIRS += \
	qtoyunda-lib \
	qtoyunda-cmd  \
	plugins	\
	GuiliGuili \
        QToyTime

win32 {
        SUBDIRS += QToyTimeWinLauncher
}


