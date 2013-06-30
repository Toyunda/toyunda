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

#include "qtoytime.h"
#include <QApplication>
#include <QTranslator>
#include "videowidget.h"
#include "../commons/sqhandlegstpath.h"
#include "generatetoyundasubtitle.h"

static QTextStream logfile;
static QTextStream cout(stdout);

void myMessageOutput(QtMsgType type, const char *msg)
{
    switch (type)
    {
        case QtDebugMsg:
            logfile << "Debug : " << msg;
            break;
        case QtCriticalMsg:
            logfile << "Critical : " << msg;
            break;
        case QtWarningMsg:
            logfile << "Warning : " << msg;
            break;
        case QtFatalMsg:
            logfile << "Fatal : " << msg;
            break;
    }
    logfile << "\n";
    logfile.flush();
    cout << msg << "\n";
    cout.flush();
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFile   mlog(qApp->applicationDirPath().toLocal8Bit() + "/qtoytimelog.txt");
    logfile.setDevice(&mlog);
    if (mlog.open(QIODevice::WriteOnly | QIODevice::Text))
        qInstallMsgHandler(myMessageOutput);
    sq_set_gstlib_env();
    sq_add_gsttoyunda_plugin_path(qApp->applicationDirPath());
    qDebug() << getenv("GST_PLUGIN_PATH");

    QString locale = QLocale::system().name().split('_').first();
    qDebug() << locale;

    QTranslator translator;
    translator.load(qApp->applicationDirPath() + "/i18n/" + locale + "_QToyTime.qm" );
    a.installTranslator(&translator);
    VideoWidget::GstInit(argc, &argv);
    QToyTime w;

    //generateToyundaSubtitle("&il &fait &beau\n&le &temps &sec\n&c'est &cool", "100 120\n200 220\n240 245\n250 255\n260 265\n270 280\n285 290\n291 297\n", "tmp.txt");
    //return 0;
    w.show();
    
    return a.exec();
}
