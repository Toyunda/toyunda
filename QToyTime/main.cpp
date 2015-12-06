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
#include "../commons/proj_handle_path.h"
#include "generatetoyundasubtitle.h"
#include <QMessageBox>
#include <QObject>

static QTextStream logfile;
static QTextStream cout(stdout);

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QByteArray localMsg = msg.toLocal8Bit();
    cout << msg;
    switch (type)
    {
        case QtDebugMsg:
            logfile << QString("Debug: %1 (%2:%3, %4)\n").arg(localMsg.constData()).arg(context.file).arg(context.line).arg(context.function);
            break;
        case QtCriticalMsg:
            logfile << QString("Critical: %1 (%2:%3, %4)\n").arg(localMsg.constData()).arg(context.file).arg(context.line).arg(context.function);
            QMessageBox::critical(NULL, QObject::tr("Critical error"), msg);
            qApp->exit(1);
            break;
        case QtWarningMsg:
            logfile << QString("Warning: %1 (%2:%3, %4)\n").arg(localMsg.constData()).arg(context.file).arg(context.line).arg(context.function);
            break;
        case QtFatalMsg:
            logfile << QString("Fatal: %1 (%2:%3, %4)\n").arg(localMsg.constData()).arg(context.file).arg(context.line).arg(context.function);
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

    qDebug() << "Starting";
    QFile   mlog(proj_logs_path() + "/qtoytimelog.txt");
    logfile.setDevice(&mlog);
    if (mlog.open(QIODevice::WriteOnly | QIODevice::Text))
        qInstallMessageHandler(myMessageOutput);
    sq_set_gstlib_env();
    sq_add_gsttoyunda_plugin_path(qApp->applicationDirPath());
    qDebug() << getenv("GST_PLUGIN_PATH");

    QString locale = QLocale::system().name().split('_').first();
    qDebug() << locale;

    QTranslator translator;
    translator.load(proj_translation_path() + "/" + locale + "_QToyTime.qm");
    a.installTranslator(&translator);
    if (VideoWidget::GstInit(argc, &argv) == false)
    {
        QMessageBox::warning(NULL, "Can't init gstreamer", "Can't init gstreamer");
        return 1;
    }
    QToyTime w;

    //generateToyundaSubtitle("&il &fait &beau\n&le &temps &sec\n&c'est &cool", "100 120\n200 220\n240 245\n250 255\n260 265\n270 280\n285 290\n291 297\n", "tmp.txt");
    //return 0;
    w.show();
    
    return a.exec();
}
