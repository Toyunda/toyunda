/*
    This file is part of GuiliGuili software
    Copyright (C) 2013  Sylvain "Skarsnik" Colinet <scolinet@gmail.com>

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

#include <QColorDialog>
#include "osdprofildialog.h"
#include <QDebug>
#include "ui_osdprofildialog.h"

osdprofildialog::osdprofildialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::osdprofildialog)
{
    ui->setupUi(this);
}

osdprofildialog::~osdprofildialog()
{
    delete ui;
}

void osdprofildialog::on_colorButton_clicked()
{
    QColorDialog    diag;

    if (diag.exec() == QDialog::Accepted)
    {
        default_color = diag.currentColor();
        ui->colorButton->setStyleSheet(QString("background-color: rgb(%1,%2,%3)").arg(diag.currentColor().red()).arg(diag.currentColor().green()).arg(diag.currentColor().blue()));
        qDebug() << QString("background-color: rgb(%1,%2,%3)").arg(diag.currentColor().red()).arg(diag.currentColor().green()).arg(diag.currentColor().blue());
    }

}
