/*
    This file is part of GuiliGuili software
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

#ifndef PROFILMPLAYER_H
#define PROFILMPLAYER_H

#include <QString>
#include <QStringList>
#include <QProcess>
#include "graphicerrorhandler.h"
#include "profil.h"

class Profilmplayer : public Profil
{
public:
    Profilmplayer();
    void    setErrorHandler(SQErrorHandler*);
    void    updateConfigDialog();
    void    updateValueFromDialog();
    bool    load(QString fileName);
    bool    save();

public slots:
    void    play(QString, QString);
    void    stop();
    bool    init();
    void    setVolume(int);
    void    dispose();
    void        on_finish(int);

 private:
    QString     m_mplayer_exec;
    QString     m_mplayer_additional_arg;
    QString     m_mplayer_WD;
    QString     m_mplayer_font_path;
    bool        m_fullscreen;
    QProcess*   m_process;

    friend class ProfilModel;
};

#endif // PROFILMPLAYER_H
