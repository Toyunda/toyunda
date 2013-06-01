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

#ifndef GSTTOYUNDAPLAYERPROFIL_H
#define GSTTOYUNDAPLAYERPROFIL_H

#include "profil.h"
#include <QProcess>
#include <QObject>

class ProfilGstToyundaPlayer : public Profil
{
Q_OBJECT
public:
    ProfilGstToyundaPlayer();
    void    setErrorHandler(SQErrorHandler *);
    void    updateConfigDialog();
    void    updateValueFromDialog();
    bool    save();
    bool    load(QString fileName);


public slots:
    void    play(QString, QString);
    void    stop();
    bool    init();
    void    setVolume(int);
    void    dispose();
    void    on_finish(int);
    void    process_error(QProcess::ProcessError);

private:
    QProcess*   m_process;
    bool        m_scaled_fullscreen;
    bool        m_fullscreen;
    QString     m_videosink;
    QString     m_audiosink;
};

#endif // GSTTOYUNDAPLAYERPROFIL_H
