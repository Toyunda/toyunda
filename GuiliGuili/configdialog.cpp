/*
    This file is part of GuiliGuili software
    Copyright (C) 2011  Sylvain "Skarsnik" Colinet <scolinet@gmail.com>

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


#include <QFileDialog>
#include "configdialog.h"

ConfigDialog::ConfigDialog()
{
    ui.setupUi(this);
}

void ConfigDialog::setProfilModel(ProfilModel *model)
{
    ui.profilTableView->setModel(model);
}

void    ConfigDialog::on_selectKaraokeDirButton_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, "Choississez le répertoire du karaoke", "", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    ui.karaokeDirLineEdit->setText(dir);
}

void ConfigDialog::on_editProfilButton_clicked()
{
    Profil *profil;

    ProfilModel* pm = static_cast<ProfilModel*>(ui.profilTableView->model());
    QModelIndex mi = ui.profilTableView->currentIndex();
    if (mi.isValid())
    {
        profil = pm->getProfil(mi.row());
        showProfilDialog(profil);
    }
}

void ConfigDialog::on_deleteProfilButton_clicked()
{
}

void ConfigDialog::on_newProfilButton_clicked()
{
}

void ConfigDialog::on_profilTableView_doubleClicked(const QModelIndex &index)
{
    Profil *profil;

    ProfilModel* pm = static_cast<ProfilModel*>(ui.profilTableView->model());
    profil = pm->getProfil(index.row());
    showProfilDialog(profil);
}

void    ConfigDialog::showProfilDialog(Profil *profil)
{
    profil->updateConfigDialog();
    profil->configDialog->exec();
}
