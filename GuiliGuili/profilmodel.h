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

#ifndef PROFILMODEL_H
#define PROFILMODEL_H

#include <QAbstractTableModel>
#include "profil.h"
#include "profilosd.h"
#include "profilmplayer.h"

class ProfilModel : public QAbstractTableModel
{
	Q_OBJECT
public:
    explicit ProfilModel(QObject *parent = 0);
    Qt::ItemFlags       flags(const QModelIndex &index) const;
    QVariant            data(const QModelIndex &index, int role) const;
    QVariant            headerData(int section, Qt::Orientation orientation, int role) const;
    int                 rowCount(const QModelIndex &parent) const;
    int                 columnCount(const QModelIndex &parent) const;
    bool                setData(const QModelIndex &index, const QVariant &value, int role);
    bool                setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int=Qt::EditRole)
    {Q_UNUSED(section); Q_UNUSED(orientation); Q_UNUSED(value); return false;}
    bool                loadProfils();
    bool                saveProfils();
    Profil*             getProfil(int);
    Profil*             getDefaultProfil();

private:
    QList<Profil*>	m_profilList;
    Profil*         m_defaultProfil;

signals:

public slots:

};

#endif // PROFILMODEL_H
