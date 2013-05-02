#include "profil.h"

const QString     Profil::ProfilDirectory = "Profils";

Profil::Profil(QObject *parent) :
    QObject(parent)
{
    m_initialised = false;
}

bool Profil::isInitialised() const
{
    return m_initialised;
}

void Profil::baseSave(QSettings *iniFile)
{
    iniFile->setValue("name", name);
    iniFile->setValue("description", description);
}

void Profil::baseLoad(QSettings *iniFile)
{
    name = iniFile->value("name").toString();
    description = iniFile->value("description").toString();
}
