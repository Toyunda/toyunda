#include <QApplication>
#include "profilosd.h"


ProfilOSD::ProfilOSD() : Profil()
{
    name = "OSD";
}

bool   ProfilOSD::init()
{
    m_qtoyunda = new QToyunda(m_errorHandler);
    QDir pluginPath = qApp->applicationDirPath();
    pluginPath.cd("plugins");
    m_qtoyunda->setPluginDirectory(pluginPath);
    if (!m_qtoyunda->loadPlugins())
       return false;
    m_qtoyunda->setPlayerName("fake");
    m_qtoyunda->setRendererName("qosd");
    QStringList rendererOption;
    rendererOption << "logo=:/main/Toyunda logo.png";
    m_qtoyunda->setRendererOption(rendererOption);
    if (!m_qtoyunda->init())
        return false;
    QObject::connect(m_qtoyunda, SIGNAL(played()), this, SIGNAL(played()));
    QObject::connect(m_qtoyunda, SIGNAL(paused()), this, SIGNAL(paused()));
    QObject::connect(m_qtoyunda, SIGNAL(stopped()), this, SIGNAL(stopped()));
    QObject::connect(m_qtoyunda, SIGNAL(finished()), this, SIGNAL(finished()));
    m_initialised = true;
    return true;
}

void    ProfilOSD::stop()
{
    m_qtoyunda->stop();
    m_qtoyunda->hideRenderer();
}

void ProfilOSD::play(QString videoPath, QString lyricsPath)
{
    if (m_qtoyunda->load(videoPath, lyricsPath))
    {
        m_qtoyunda->play();
        m_qtoyunda->showRenderer();
    }
    else
        emit error_occured();
}

void ProfilOSD::setErrorHandler(SQErrorHandler *err)
{
    m_errorHandler = err;
}

void ProfilOSD::updateConfigDialog()
{
}

void ProfilOSD::updateValueFromDialog()
{
}

void ProfilOSD::setVolume(int vol)
{
    m_qtoyunda->setVolume(vol);
}

void ProfilOSD::dispose()
{
    m_qtoyunda->dispose();
}
