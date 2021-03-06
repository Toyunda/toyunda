/*
    This file is part of GuiliGuili software
    Copyright (C) 2011  Sylvain "Skarsnik" Colinet <scolinet@gmail.com>

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

#include <QApplication>
#include "GuiliGuili.h"
#include <QTextStream>
#include "sqhandlegstpath.h"

static QTextStream logfile;
static QTextStream cout(stdout);

void myMessageOutput(QtMsgType type, const QMessageLogContext& context, const QString& msg)
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


int main(int argc, char** argv)
{
    QApplication app(argc, argv);


    QFile   mlog(qApp->applicationDirPath().toLocal8Bit() + "/log.txt");
    logfile.setDevice(&mlog);
    if (mlog.open(QIODevice::WriteOnly | QIODevice::Text))
        qInstallMessageHandler(myMessageOutput);
    sq_set_gstlib_env();
    GuiliGuili foo;
    foo.show();
    return app.exec();
}
