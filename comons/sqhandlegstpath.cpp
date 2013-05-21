#include <stdlib.h>
#include <QDebug>
#include <QApplication>
#include "sqhandlegstpath.h"

void    sq_set_gstlib_env()
{
#ifdef Q_WS_WIN32
        char    *mPath, *mGstRoot;
        mPath = getenv("PATH");
        mGstRoot = getenv("GSTREAMER_SDK_ROOT_X86");
        if (mGstRoot == NULL)
        {
            QString tmpL = qApp->applicationDirPath().toLocal8Bit() + "\\gstreamer-sdk\\0.10\\x86\\";
            mGstRoot = (char *)malloc(strlen(tmpL.toLocal8Bit().data()) + 1);
            strcpy(mGstRoot, tmpL.toLocal8Bit().data());
        }
        QString mTmp = QString("PATH=%1;%2\\bin\\;%2\\lib").arg(mPath).arg(mGstRoot);
        qDebug() << mTmp;
        _putenv(mTmp.toLocal8Bit().data());
        //free(mGstRoot);
#endif
}

void    sq_add_gsttoyunda_plugin_path(QString appPath)
{
    _putenv(QString("GST_PLUGIN_PATH=" + sq_get_gsttoyunda_plugin_path(appPath)).toLatin1().constData());
}

QString sq_get_gsttoyunda_plugin_path(QString appPath)
{
   return (appPath + "/gsttoyunda/");
}
