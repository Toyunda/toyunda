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

#include "gstplayerprofildialog.h"
#include "ui_gstplayerprofildialog.h"

GstPlayerProfilDialog::GstPlayerProfilDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GstPlayerProfilDialog)
{
    ui->setupUi(this);
    fillComboxBox();
}

GstPlayerProfilDialog::~GstPlayerProfilDialog()
{
    delete ui;
}

void GstPlayerProfilDialog::updateValue()
{
    ui->fsCheckBox->setChecked(fullscreen);
    ui->sfscheckBox->setChecked(scaled_fullscreen);
    for (int i = 0; i < ui->audioSinkComboBox->count(); i++)
    {
        if (ui->audioSinkComboBox->itemText(i) == audiosink)
        {
            ui->audioSinkComboBox->setCurrentIndex(i);
            break;
        }
    }
    for (int i = 0; i < ui->videoSinkComboBox->count(); i++)
    {
        if (ui->videoSinkComboBox->itemText(i) == videosink)
        {
            ui->videoSinkComboBox->setCurrentIndex(i);
            break;
        }
    }
}

void GstPlayerProfilDialog::fillComboxBox()
{
    QStringList videosinkList;
    QStringList audiosinkList;

    audiosinkList << "autoaudiosink";
#ifdef Q_WS_X11
    videosinkList << "autovideosink" << "ximagesink" << "xvimagesink" << "glimagesink" << "cluttersink";
    audiosinkList << "alsasink" << "pulsesink";
#endif
#ifdef Q_WS_WIN32
    videosinkList << "directdrawsink" << "d3dvideosink" << "dshowvideosink" << "cluttersink";
    audiosinkList << "directsoundsink";
#endif
    ui->videoSinkComboBox->clear();
    ui->audioSinkComboBox->clear();
    ui->videoSinkComboBox->addItems(videosinkList);
    ui->audioSinkComboBox->addItems(audiosinkList);
}

void GstPlayerProfilDialog::on_fsCheckBox_stateChanged(int arg1)
{
    if (arg1 == Qt::Checked || arg1 == Qt::Unchecked)
        fullscreen = ui->fsCheckBox->isChecked();
}

void GstPlayerProfilDialog::on_sfscheckBox_stateChanged(int arg1)
{
    if (arg1 == Qt::Checked || arg1 == Qt::Unchecked)
        scaled_fullscreen = ui->sfscheckBox->isChecked();
}
