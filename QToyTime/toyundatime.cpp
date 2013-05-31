#include "toyundatime.h"
#include <QFile>
#include <QSettings>
#include <QFileInfo>
#include <QDebug>

ToyundaTime::ToyundaTime()
{
    m_lyrFine = true;
    m_frmFine = true;
}

ToyundaTime::ToyundaTime(QString ini)
{
    m_lyrFine = true;
    m_frmFine = true;

    QSettings    mini(ini, QSettings::IniFormat);
    m_iniFile = QFileInfo(ini).fileName();
    m_baseDir = QFileInfo(ini).absolutePath();
    m_videoFile = mini.value("movie/aviname").toString();
    m_lyrFile = mini.value("qtoytime/lyrfile").toString();
    m_frmFile = mini.value("qtoytime/frmfile").toString();
    m_subFile = mini.value("subtitles/file").toString();

    loadLyrFrm();
}

bool ToyundaTime::save(const QString &frmText, const QString &lyrText)
{
    QSettings*   ini;
    if (QFileInfo(m_iniFile).isRelative())
        ini = new QSettings(m_baseDir + "/" + m_iniFile, QSettings::IniFormat);
    else
        ini = new QSettings(m_iniFile, QSettings::IniFormat);
    ini->setValue("movie/aviname", QFileInfo(m_videoFile).fileName());
    ini->setValue("subtitles/file", QFileInfo(m_subFile).fileName());
    ini->setValue("qtoytime/lyrfile", QFileInfo(m_lyrFile).fileName());
    ini->setValue("qtoytime/frmfile", QFileInfo(m_frmFile).fileName());

    if (!frmText.isEmpty())
    {
        QFile   fi(m_frmFile);
        fi.open(QIODevice::WriteOnly | QIODevice::Text);
        fi.write(frmText.toLocal8Bit());
        fi.close();
    }
    if (!lyrText.isEmpty())
    {
        QFile   fi(m_lyrFile);
        fi.open(QIODevice::WriteOnly | QIODevice::Text);
        fi.write(lyrText.toLocal8Bit());
        fi.close();
    }
}

bool ToyundaTime::loadLyrFrm()
{
    QFile   fi(m_baseDir + "/" + m_frmFile);
    if (fi.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        m_frmText = fi.readAll();
        fi.close();
    }
    else
    {
        m_frmFine = false;
        qDebug() << fi.errorString();
    }
    fi.setFileName(m_baseDir + "/" + m_lyrFile);
    if (fi.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        m_lyrText = fi.readAll();
        fi.close();
    }
    else
    {
        m_lyrFine = false;
        qDebug() << fi.errorString();
    }
}

QString ToyundaTime::getLyrText() const
{
    return m_lyrText;
}

QString ToyundaTime::getFrmText() const
{
    return m_frmText;
}

QString ToyundaTime::subFile() const
{
    return m_subFile;
}

void ToyundaTime::setSubFile(const QString &subFile)
{
    m_subFile = subFile;
}

QString ToyundaTime::videoFile() const
{
    return m_videoFile;
}

void ToyundaTime::setVideoFile(const QString &videoFile)
{
    m_videoFile = videoFile;
}

QString ToyundaTime::lyrFile() const
{
    return m_lyrFile;
}

void ToyundaTime::setLyrFile(const QString &lyrFile)
{
    m_lyrFile = lyrFile;
}

QString ToyundaTime::frmFile() const
{
    return m_frmFile;
}

void ToyundaTime::setFrmFile(const QString &frmFile)
{
    m_frmFile = frmFile;
}

QString ToyundaTime::iniFile() const
{
    return m_iniFile;
}

void ToyundaTime::setIniFile(const QString &iniFile)
{
    m_iniFile = iniFile;
}


bool ToyundaTime::frmFine() const
{
    return m_frmFine;
}

bool ToyundaTime::lyrFine() const
{
    return m_lyrFine;
}

QString ToyundaTime::baseDir() const
{
    return m_baseDir;
}

void ToyundaTime::setBaseDir(const QString &baseDir)
{
    m_baseDir = baseDir;
}
