#include <QApplication>
#include "profilosd.h"


ProfilOSD::ProfilOSD()
{
    name = "OSD";
    m_errorHandler = new GraphicErrorHandler();
    m_qtoyunda = new QToyunda(m_errorHandler);
}

bool   ProfilOSD::init()
{
    QDir pluginPath = qApp->applicationDirPath();
    pluginPath.cd("plugins");
    m_qtoyunda->setPluginDirectory(pluginPath);
    m_qtoyunda->loadPlugins();
    m_qtoyunda->setPlayerName("qgstaudio");
    m_qtoyunda->setRendererName("qosd");
    QStringList rendererOption;
    rendererOption << "logo=:/main/Toyunda logo.png";
    m_qtoyunda->setRendererOption(rendererOption);
    m_qtoyunda->init();
    QObject::connect(m_qtoyunda, SIGNAL(played()), this, SIGNAL(played()));
    QObject::connect(m_qtoyunda, SIGNAL(paused()), this, SIGNAL(paused()));
    QObject::connect(m_qtoyunda, SIGNAL(stopped()), this, SIGNAL(stopped()));
    QObject::connect(m_qtoyunda, SIGNAL(finished()), this, SIGNAL(finished()));
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
}

void ProfilOSD::setEH(GraphicErrorHandler *err)
{
    m_qtoyunda = new QToyunda(err);
}

void ProfilOSD::setVolume(int vol)
{
    m_qtoyunda->setVolume(vol);
}

void ProfilOSD::dispose()
{
    m_qtoyunda->dispose();
}
