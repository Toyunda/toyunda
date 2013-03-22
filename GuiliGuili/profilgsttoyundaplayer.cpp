#include "profilgsttoyundaplayer.h"
#include <QApplication>
#include <QDebug>

ProfilGstToyundaPlayer::ProfilGstToyundaPlayer() : Profil()
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
    QString tmp = qApp->applicationDirPath();
    qDebug() << "Application path : " << tmp;
    tmp.append("/toyunda-player");
    qDebug() << tmp;
    m_process->start(tmp, arg);
}

void ProfilGstToyundaPlayer::stop()
{
    m_process->kill();
}


bool ProfilGstToyundaPlayer::init()
{
    m_process = new QProcess(this);
    m_process->setWorkingDirectory(qApp->applicationDirPath());
    QObject::connect(m_process, SIGNAL(started()), this, SIGNAL(played()));
    QObject::connect(m_process, SIGNAL(finished(int)), this, SIGNAL(finished()));
    QObject::connect(m_process, SIGNAL(error(QProcess::ProcessError)), this, SLOT(process_error(QProcess::ProcessError)));
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

void ProfilGstToyundaPlayer::process_error(QProcess::ProcessError err)
{
    err = err;
    qDebug() << m_process->errorString();
}


void ProfilGstToyundaPlayer::updateConfigDialog()
{
}


void ProfilGstToyundaPlayer::updateValueFromDialog()
{
}


