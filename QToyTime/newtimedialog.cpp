/*
    This file is part of QToyunda software
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

#include "newtimedialog.h"
#include "ui_newtimedialog.h"
#include <QFileDialog>
#include <QDebug>

NewTimeDialog::NewTimeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewTimeDialog)
{
    m_frmChoosed = false;
    m_lyrChoosed = false;
    m_iniChoosed = false;
    ui->setupUi(this);
}

NewTimeDialog::~NewTimeDialog()
{
    delete ui;
}

void NewTimeDialog::on_titleEdit_textEdited(const QString &arg1)
{
    if (!m_frmChoosed)
        ui->frmEdit->setText(arg1 + ".frm");
    if (!m_lyrChoosed)
        ui->lyrEdit->setText(arg1 + ".lyr");
    if (!m_iniChoosed)
        ui->fileNameEdit->setText(ui->prefixEdit->text() + " - " + arg1 + ".ini");
    ui->subtitleEdit->setText(arg1 + ".txt");
}

void NewTimeDialog::on_prefixEdit_textEdited(const QString &arg1)
{
    if (!m_iniChoosed)
        ui->fileNameEdit->setText(arg1 + " - " + ui->titleEdit->text() + ".ini");
}

void NewTimeDialog::on_videoChooseButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Choose video"),
                                                    "", "");
    if (!fileName.isNull())
    {
        ui->videoEdit->setText(fileName);
        setBaseDir(fileName);
    }
}

void NewTimeDialog::on_lyrChooseButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Choose lyr file"), "", "Lyrics file (*.lyr)");
    if (!fileName.isNull())
    {
        ui->lyrEdit->setText(fileName);
        m_lyrChoosed = true;
    }
}

void NewTimeDialog::on_frmChooseButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                            tr("Choose FRM file"), "", "FRM file (*.frm)");
    if (!fileName.isNull())
    {
        ui->frmEdit->setText(fileName);
        m_frmChoosed = true;
    }
}

void NewTimeDialog::on_buttonBox_accepted()
{
    lyrFile = ui->lyrEdit->text();
    frmFile = ui->frmEdit->text();
    videoFile = ui->videoEdit->text();
    iniFile = ui->fileNameEdit->text();
    subFile = ui->subtitleEdit->text();
}

void NewTimeDialog::setBaseDir(QString file)
{
    QFileInfo   fi(file);

    baseDir = fi.path();
}
