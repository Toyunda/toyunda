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


#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include <QDialog>
#include "ui_configDialog.h"
#include "profilmanager.h"

class ConfigDialog : public QDialog
{
    Q_OBJECT
    public:
        ConfigDialog();
	Ui::configDialog	ui;
	ProfilManager		profilManager;

    private slots:
        void    on_selectKaraokeDirButton_clicked();
/*	void	on_editProfilButton_clicked();
	void	on_deleteProfilButton_clicked();
	void	on_newProfilButton_clicked();*/
};

#endif // CONFIGDIALOG_H
