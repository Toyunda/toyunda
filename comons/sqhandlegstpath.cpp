/*
    This file is part of QToyunda software
    Copyright (C) 2013  Sylvain "Skarsnik" Colinet <scolinet@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdlib.h>
#include <QDebug>
#include <QCoreApplication>
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
        QString mTmp = QString("PATH=%%2\\bin\\;%2\\lib;%1").arg(mPath).arg(mGstRoot);
        qDebug() << mTmp;
        _putenv(mTmp.toLocal8Bit().data());
        //free(mGstRoot);
#endif
}

void    sq_add_gsttoyunda_plugin_path(QString appPath)
{
#ifdef Q_WS_WIN32
    _putenv(QString("GST_PLUGIN_PATH=" + sq_get_gsttoyunda_plugin_path(appPath)).toLatin1().constData());
#endif
#ifdef Q_WS_X11
    setenv("GST_PLUGIN_PATH", sq_get_gsttoyunda_plugin_path(appPath).toLatin1().constData(), 1);
#endif
}

QString sq_get_gsttoyunda_plugin_path(QString appPath)
{
   return (appPath + "/gsttoyunda/");
}
