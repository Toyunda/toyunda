#include "profil.h"

Profil::Profil(QObject *parent) :
    QObject(parent)
{
    m_initialised = false;
}

bool Profil::isInitialised() const
{
    return m_initialised;
}
