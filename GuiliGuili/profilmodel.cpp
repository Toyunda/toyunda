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

#include "profilmodel.h"
#include "profilgsttoyundaplayer.h"

ProfilModel::ProfilModel(QObject *parent) :
    QAbstractTableModel(parent)
{

}

Qt::ItemFlags ProfilModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags theFlags = QAbstractTableModel::flags(index);
    if (index.isValid())
        theFlags |= Qt::ItemIsSelectable | Qt::ItemIsEditable
                 | Qt::ItemIsEnabled;
    return theFlags;
}

QVariant ProfilModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_profilList.count()
            || index.column() < 0 || index.column() >= 2)
        return QVariant();
    const Profil *prof = m_profilList.at(index.row());
    if (role == Qt::DisplayRole || role == Qt::EditRole)
    {
        switch(index.column()) {
            case 0 : return prof->name;
            case 1 : return prof->description;
            default : Q_ASSERT(false);
        }
    }
    return QVariant();
}

QVariant ProfilModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();
    if (orientation == Qt::Horizontal)
    {
        switch(section) {
        case 0 : return "Name";
        case 1 : return "Description";
        default: Q_ASSERT(false);
        }
    }
    return section + 1;
}

int ProfilModel::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return m_profilList.count();
    return 0;
}

int ProfilModel::columnCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return 2;
    return 0;
}

bool ProfilModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid() || index.row() >= m_profilList.count()
            || index.column() < 0 || index.column() >= 2 || role != Qt::EditRole)
        return false;
    Profil *profil = m_profilList[index.row()];
    switch(index.column()){
    case 0 : profil->name = value.toString(); break;
    case 1 : profil->description = value.toString(); break;
    default : Q_ASSERT(false);
    }
    emit dataChanged(index, index);
    return true;
}

#define PROFIL_DIR "Profils"

bool ProfilModel::loadProfils()
{
    Profil *osdProfil = new	ProfilOSD();
    osdProfil->baseType = Profil::OSD;
    Profil	*mpProfil = new Profilmplayer;
    mpProfil->baseType = Profil::MPLAYER;
    mpProfil->name = "Default MplayerToyunda";
    mpProfil->description = "Default MPlayer toyunda profil";
    osdProfil->name = "Default OSD";
    osdProfil->description = "The default OSD profil";
    Profil  *gstprofil = new ProfilGstToyundaPlayer();
    gstprofil->name = "Gst toyunda player";
    gstprofil->description = "Default gst toyunda player";
    gstprofil->baseType = Profil::GSTPLAYER;
    m_defaultProfil = osdProfil;
    m_profilList.append(osdProfil);
    m_profilList.append(mpProfil);
    m_profilList.append(gstprofil);
    return true;
}

bool ProfilModel::saveProfils()
{
    return true;
}

Profil *ProfilModel::getProfil(int index)
{
    return m_profilList[index];
}

Profil* ProfilModel::getDefaultProfil()
{
    return m_defaultProfil;
}
