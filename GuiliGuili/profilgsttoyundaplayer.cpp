#include "profilgsttoyundaplayer.h"

ProfilGstToyundaPlayer::ProfilGstToyundaPlayer()
{
    name = "Gsttoyunda Player";
    configDialog = new QDialog();
}

void ProfilGstToyundaPlayer::setErrorHandler(SQErrorHandler *)
{
}


void ProfilGstToyundaPlayer::play(QString video, QString lyrics)
{
    QStringList arg;
    arg << video << lyrics;
    m_process->start("toyunda-player", arg);
}

void ProfilGstToyundaPlayer::stop()
{
    m_process->kill();
}


bool ProfilGstToyundaPlayer::init()
{
    m_process = new QProcess(this);
    QObject::connect(m_process, SIGNAL(started()), this, SIGNAL(played()));
    QObject::connect(m_process, SIGNAL(finished(int)), this, SIGNAL(finished()));
    m_initialised = true;
    return true;
}


void ProfilGstToyundaPlayer::setVolume(int)
{
}

void ProfilGstToyundaPlayer::dispose()
{
}

void ProfilGstToyundaPlayer::on_finish(int)
{
}


void ProfilGstToyundaPlayer::updateConfigDialog()
{
}


void ProfilGstToyundaPlayer::updateValueFromDialog()
{
}


