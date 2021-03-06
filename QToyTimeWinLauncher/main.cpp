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

#include <QCoreApplication>
#include <QProcess>
#include "../commons/sqhandlegstpath.h"
#include "process.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    
    sq_set_gstlib_env();
    //freopen("qtoytime-out.txt","w", stdout);
    _execv(QString(qApp->applicationDirPath() + "/_binQToyTime.exe").toLatin1().data(), argv);
    //fclose(stdout);
    return 0;
}
