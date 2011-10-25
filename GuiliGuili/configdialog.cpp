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

void    ConfigDialog::on_selectKaraokeDirButton_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, "Choississez le répertoire du karaoke", "", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    ui.karaokeDirLineEdit->setText(dir);
}
