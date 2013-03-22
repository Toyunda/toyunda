/*
    This file is part of GuiliGuili software
    Copyright (C) 2012  Sylvain "Skarsnik" Colinet <scolinet@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "profilmanager.h"
#include "profilmplayer.h"
#include "profilosd.h"

ProfilManager::ProfilManager()
{
}

bool ProfilManager::loadProfil()
{
	Profil *osdProfil = new	ProfilOSD();
	osdProfil->baseType = Profil::OSD;
	Profil	*mpProfil = new Profilmplayer;
	mpProfil->baseType = Profil::MPLAYER;
	m_profilList.append(osdProfil);
	m_profilList.append(mpProfil);
    return true;
}

const QList<Profil *> ProfilManager::getProfilList() const
{
	return m_profilList;
}

bool ProfilManager::addProfil(Profil *)
{
    return true;
}

bool ProfilManager::deleteProfil(Profil *)
{
    return true;
}
