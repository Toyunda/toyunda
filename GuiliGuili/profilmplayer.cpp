#include <QDebug>
#include "profilmplayer.h"
#include "mplayerprofildialog.h"

Profilmplayer::Profilmplayer() : Profil()
{
    name = "MPlayer-Toyunda";
    m_mplayer_exec = "/home/skarsnik/compile/MPlayer-0.9X-Toyunda-b8dev-ffmpeg-xv-alsa/mplayer-toyunda";
    m_mplayer_default_arg << "-fs" << "-font" << "/home/skarsnik/compile/MPlayer-0.9X-Toyunda-b8dev-ffmpeg-xv-alsa/fonts/font.desc";
    m_mplayer_exec_path = "/home/skarsnik/compile/MPlayer-0.9X-Toyunda-b8dev-ffmpeg-xv-alsa/tga";
    configDialog = new mplayerProfilDialog();
}

void Profilmplayer::setErrorHandler(SQErrorHandler *)
{
}

void Profilmplayer::updateConfigDialog()
{
    mplayerProfilDialog *diag = static_cast<mplayerProfilDialog*>(configDialog);
    diag->mplayerAdditionnalArg = "";
    diag->mplayerExecPath = m_mplayer_exec;
    diag->mplayerWDPath = m_mplayer_exec_path;
    diag->mplayerFontPath = m_mplayer_font_path;
    diag->updateValue();
}

void Profilmplayer::updateValueFromDialog()
{
    mplayerProfilDialog *diag = static_cast<mplayerProfilDialog*>(configDialog);
    m_mplayer_exec = diag->mplayerExecPath;
    m_mplayer_exec_path = diag->mplayerWDPath;
    m_mplayer_font_path = diag->mplayerFontPath;
}

void Profilmplayer::play(QString video, QString lyrics)
{
    QStringList arg = m_mplayer_default_arg;
    arg << "-sub" << lyrics << video;
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
    m_process = new QProcess(this);
    m_process->setWorkingDirectory(m_mplayer_exec_path);
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
