#include "profilgsttoyundaplayer.h"
#include "gstplayerprofildialog.h"
#include <QApplication>
#include <QDebug>

ProfilGstToyundaPlayer::ProfilGstToyundaPlayer() : Profil()
{
    m_fullscreen = false;
    m_scaled_fullscreen = true;
    configDialog = new GstPlayerProfilDialog();
}

void ProfilGstToyundaPlayer::setErrorHandler(SQErrorHandler *)
{
}


void ProfilGstToyundaPlayer::play(QString video, QString lyrics)
{
    QStringList arg;
    if (m_fullscreen)
        arg << "-f";
    if (!m_scaled_fullscreen)
        arg << "-e";
    if (!m_audiosink.isEmpty())
        arg << "--audioouput" << m_audiosink;
    if (!m_videosink.isEmpty())
        arg << "--videooutput" << m_videosink;
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
    env << "GST_PLUGIN_PATH=" + qApp->applicationDirPath() + "/gsttoyunda/";
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
    GstPlayerProfilDialog   *diag = static_cast<GstPlayerProfilDialog*>(configDialog);
    diag->fullscreen = m_fullscreen;
    diag->scaled_fullscreen = m_scaled_fullscreen;
    diag->videosink = m_videosink;
    diag->audiosink = m_audiosink;
    diag->updateValue();
}


void ProfilGstToyundaPlayer::updateValueFromDialog()
{
    GstPlayerProfilDialog   *diag = static_cast<GstPlayerProfilDialog*>(configDialog);
    m_fullscreen = diag->fullscreen;
    m_scaled_fullscreen = diag->scaled_fullscreen;
    m_videosink = diag->videosink;
    m_audiosink = diag->audiosink;
}

bool ProfilGstToyundaPlayer::save()
{
    QSettings   conf(fileName, QSettings::IniFormat);
    baseSave(&conf);
    conf.setValue("fullscreen", m_fullscreen);
    conf.setValue("scaled_fullscreen", m_scaled_fullscreen);
    conf.setValue("audiosink", m_audiosink);
    conf.setValue("videosink", m_videosink);
    return true;
}

bool ProfilGstToyundaPlayer::load(QString fileName)
{
    QSettings   conf(fileName, QSettings::IniFormat);
    baseLoad(&conf);
    m_fullscreen = conf.value("fullscreen").toBool();
    m_scaled_fullscreen = conf.value("scaled_fullscreen").toBool();
    m_videosink = conf.value("videosink").toString();
    m_audiosink = conf.value("audiosink").toString();
    return true;
}


