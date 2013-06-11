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

#include <QDebug>
#include "profilmplayer.h"
#include "mplayerprofildialog.h"

Profilmplayer::Profilmplayer() : Profil()
{
    configDialog = new mplayerProfilDialog();
    m_fullscreen = true;
    m_process = new QProcess();
}

void Profilmplayer::setErrorHandler(SQErrorHandler *)
{
}

void Profilmplayer::updateConfigDialog()
{
    mplayerProfilDialog *diag = static_cast<mplayerProfilDialog*>(configDialog);
    diag->mplayerAdditionnalArg = m_mplayer_additional_arg;
    diag->mplayerExecPath = m_mplayer_exec;
    diag->mplayerWDPath = m_mplayer_WD;
    diag->mplayerFontPath = m_mplayer_font_path;
    diag->fullscreen = m_fullscreen;
    diag->updateValue();
}

void Profilmplayer::updateValueFromDialog()
{
    mplayerProfilDialog *diag = static_cast<mplayerProfilDialog*>(configDialog);
    m_mplayer_additional_arg = diag->mplayerAdditionnalArg;
    m_mplayer_exec = diag->mplayerExecPath;
    m_mplayer_WD = diag->mplayerWDPath;
    m_mplayer_font_path = diag->mplayerFontPath;
    m_fullscreen = diag->fullscreen;
    m_process->setWorkingDirectory(m_mplayer_WD);
}

bool Profilmplayer::load(QString fileName)
{
    QSettings   conf(fileName, QSettings::IniFormat);
    baseLoad(&conf);
    m_mplayer_exec = conf.value("mplayer_exec").toString();
    m_mplayer_additional_arg = conf.value("mplayer_add_args").toString();
    m_mplayer_font_path = conf.value("mplayer_font_path").toString();
    m_mplayer_WD = conf.value("mplayer_WD").toString();
    m_fullscreen = conf.value("fullscreen").toBool();
    return true;
}

bool Profilmplayer::save()
{
    QSettings   conf(fileName, QSettings::IniFormat);
    conf.sync();
    if (conf.status() != QSettings::NoError)
        return false;
    baseSave(&conf);
    conf.setValue("mplayer_exec", m_mplayer_exec);
    conf.setValue("mplayer_add_args", m_mplayer_additional_arg);
    conf.setValue("mplayer_font_path", m_mplayer_font_path);
    conf.setValue("mplayer_WD", m_mplayer_WD);
    conf.setValue("fullscreen", m_fullscreen);
    return true;
}

void Profilmplayer::play(QString video, QString lyrics)
{
    QStringList arg;
    if (!m_mplayer_font_path.isEmpty())
        arg << "-font" << m_mplayer_font_path;
    if (m_fullscreen)
        arg << "-fs";
    arg << "-sub" << lyrics << video;
    if (!m_mplayer_additional_arg.isEmpty())
        arg << m_mplayer_additional_arg.split(";");
    qDebug() << m_mplayer_exec << arg;
    m_process->start(m_mplayer_exec, arg);
}

void Profilmplayer::stop()
{
    m_process->kill();
    //emit stopped();
}

void    Profilmplayer::on_finish(int po)
{
	Q_UNUSED(po);
    emit finished();
}

bool Profilmplayer::init()
{
    //m_process = new QProcess(this);
    m_process->setWorkingDirectory(m_mplayer_WD);
    QObject::connect(m_process, SIGNAL(started()), this, SIGNAL(played()));
    QObject::connect(m_process, SIGNAL(finished(int)), this, SIGNAL(finished()));
    m_initialised = true;
    return true;
}

void Profilmplayer::setVolume(int)
{
}

void Profilmplayer::dispose()
{
}
