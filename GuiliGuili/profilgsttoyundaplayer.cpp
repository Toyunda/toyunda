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
    tmp.append("/toyunda-player");
    qDebug() << "Starting : " << tmp << arg;
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
    QStringList env = QProcess::systemEnvironment();
    env << "GST_PLUGIN_PATH=" + qApp->applicationDirPath();
    m_process->setEnvironment(env);
    QObject::connect(m_process, SIGNAL(started()), this, SIGNAL(played()));
    QObject::connect(m_process, SIGNAL(finished(int)), this, SLOT(on_finish(int)));
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
    qDebug() << m_process->errorString();
    qDebug() << m_process->readAll();
    qDebug() << m_process->exitCode();
    emit finished();
}

void ProfilGstToyundaPlayer::process_error(QProcess::ProcessError err)
{
    err = err;
    qDebug() << m_process->readAllStandardError();
    qDebug() << m_process->readAllStandardOutput();
    qDebug() << m_process->errorString();
}


void ProfilGstToyundaPlayer::updateConfigDialog()
{
}


void ProfilGstToyundaPlayer::updateValueFromDialog()
{
}

bool ProfilGstToyundaPlayer::save(QString fileName)
{
    return true;
}

bool ProfilGstToyundaPlayer::load(QString fileName)
{
    return true;
}


