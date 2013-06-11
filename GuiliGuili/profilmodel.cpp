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

#include <QApplication>
#include "profilmodel.h"
#include "profilgsttoyundaplayer.h"
#include <QSettings>

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

#define PROFIL_TYPE_STRING "ProfilType"
#define PROFIL_TYPE_QOSD "ProfilQOSD"
#define PROFIL_TYPE_MPLAYER "ProfilMplayer"
#define PROFIL_TYPE_GSTOYUNDA "ProfilGSToyunda"

bool ProfilModel::loadProfils()
{
    if (!QFile::exists(Profil::ProfilDirectory))
        if (!QDir(qApp->applicationDirPath()).mkdir(Profil::ProfilDirectory))
            return createDefaultProfils(false);

    QDir    pdir(qApp->applicationDirPath().toLocal8Bit() + "/" + Profil::ProfilDirectory);
    QStringList dirFilter;
    dirFilter << "*.ini";
    pdir.setNameFilters(dirFilter);
    QStringList strlt = pdir.entryList();
    QStringListIterator it(strlt);
    if (strlt.isEmpty())
        return createDefaultProfils();
    while (it.hasNext())
    {
        Profil* newProfil;
        QString fileName = qApp->applicationDirPath().toLocal8Bit() + "/" + Profil::ProfilDirectory + "/" + it.next();
        QSettings* fileIni = new QSettings(fileName, QSettings::IniFormat);
        fileIni->sync();
        if (fileIni->status() != QSettings::NoError)
            return false;
        if (fileIni->value(PROFIL_TYPE_STRING).toString() == PROFIL_TYPE_GSTOYUNDA)
                newProfil = new ProfilGstToyundaPlayer;
        if (fileIni->value(PROFIL_TYPE_STRING).toString() == PROFIL_TYPE_MPLAYER)
                newProfil = new Profilmplayer;
        if (fileIni->value(PROFIL_TYPE_STRING).toString() == PROFIL_TYPE_QOSD)
                newProfil = new ProfilOSD;
        newProfil->fileName = fileName;
        delete fileIni;
        newProfil->load(fileName);
        m_profilList.append(newProfil);
    }
    m_defaultProfil = m_profilList.first();
    return true;
}

bool ProfilModel::saveProfils()
{
    QListIterator<Profil*>it(m_profilList);
    while (it.hasNext())
    {
        Profil* myProf = it.next();
        if (myProf->fileName.isEmpty())
            myProf->fileName = qApp->applicationDirPath().toLocal8Bit() + "/" + Profil::ProfilDirectory + "/" + myProf->name + ".ini";
        QSettings*  mConf = new QSettings(myProf->fileName, QSettings::IniFormat);
        if (myProf->baseType == Profil::OSD)
            mConf->setValue(PROFIL_TYPE_STRING, PROFIL_TYPE_QOSD);
        if (myProf->baseType == Profil::MPLAYER)
            mConf->setValue(PROFIL_TYPE_STRING, PROFIL_TYPE_MPLAYER);
        if (myProf->baseType == Profil::GSTPLAYER)
            mConf->setValue(PROFIL_TYPE_STRING, PROFIL_TYPE_GSTOYUNDA);
        delete mConf;
        if (!myProf->save())
            return false;
    }
    return true;
}

bool ProfilModel::createDefaultProfils(bool createFile)
{
    qDebug() << "Creating default Profil";
    Profil *osdProfil = new	ProfilOSD();
    osdProfil->baseType = Profil::OSD;
    Profil	*mpProfil = new Profilmplayer;
    mpProfil->baseType = Profil::MPLAYER;
    mpProfil->name = "Default MplayerToyunda";
    mpProfil->description = "Default MPlayer toyunda profil";

    if (QFile::exists(qApp->applicationDirPath() + "/MPlayer-Toyunda/"))
    {
        QString mpPath = qApp->applicationDirPath() + "/MPlayer-Toyunda/";
        Profilmplayer *mptProfil = static_cast<Profilmplayer*>(mpProfil);
        mptProfil->m_mplayer_exec = mpPath + "/mplayer-toyunda.exe";
        mptProfil->m_mplayer_WD = mpPath;
        mptProfil->m_mplayer_font_path = mpPath + "/font/font.desc";
        mptProfil->m_mplayer_additional_arg = "-include;"+ mpPath + "/config.txt";
    }

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
    if (createFile)
    {
        mpProfil->fileName = qApp->applicationDirPath().toLocal8Bit() + "/" + Profil::ProfilDirectory + "/" + "DefaultMplayer.ini";
        osdProfil->fileName = qApp->applicationDirPath().toLocal8Bit() + "/" + Profil::ProfilDirectory + "/" +  "DefaultOSD.ini";
        gstprofil->fileName = qApp->applicationDirPath().toLocal8Bit() + "/" + Profil::ProfilDirectory + "/" + "DefaultGST.ini";
        return saveProfils();
    }
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

void ProfilModel::setDefaultProfil(QString name)
{
    foreach(Profil *prof, m_profilList)
    {
        if (prof->name == name)
        {
            m_defaultProfil = prof;
            break;
        }
    }
}

int ProfilModel::getProfilIndex(Profil *prof)
{
    for (int i = 0; i < m_profilList.count(); i++)
        if (m_profilList[i] == prof)
            return i;
    return -1;
}
