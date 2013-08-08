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

#include "configdialog.h"
#include "ui_configdialog.h"
#include <QDebug>
#include <QFileDialog>
#include <QStandardItemModel>

configDialog::configDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::configDialog)
{
    ui->setupUi(this);
#ifdef  Q_WS_WIN32
    ui->videosinkComboBox->addItem("directdrawsink");
    ui->videosinkComboBox->addItem("d3dvideosink");
    ui->videosinkComboBox->addItem("dshowvideosink");
#endif
#ifdef  Q_WS_X11
    ui->videosinkComboBox->addItem("autovideosink");
    ui->videosinkComboBox->addItem("xvimagesink");
    ui->videosinkComboBox->addItem("ximagesink");
    ui->videosinkComboBox->addItem("glimagesink");
#endif
    ui->videosinkComboBox->addItem("cluttersink");

    QStandardItemModel*  mod = new QStandardItemModel();
    QStandardItem*  settings = new QStandardItem(QIcon(":/icons/redhat-system_settings.png"), tr("Settings"));
    QStandardItem*  behavior = new QStandardItem(QIcon(":/icons/xfce4-settings.png"), tr("Behavior"));
    QStandardItem*  lookandfeel = new QStandardItem(QIcon(":/icons/format-stroke-color.png"), tr("Look and Feel"));
    mod->appendRow(behavior);
    mod->appendRow(lookandfeel);
    mod->appendRow(settings);
    ui->listView->setModel(mod);
    ui->fontPreviewEdit->setText("Twilight is best pony");
}

configDialog::~configDialog()
{
    delete ui;
}

void configDialog::setReplaceMode(bool r)
{
    replaceMode = r;
    ui->frameMarginSpinBox->setEnabled(r);
    ui->replaceCheckBox->setChecked(r);
    ui->classicCheckBox->setChecked(!r);
}

void    configDialog::setVideoSink(QString vsink)
{
    for (int i = 0; i < ui->videosinkComboBox->count(); i++)
    {
        if (ui->videosinkComboBox->itemText(i) == vsink)
        {
            ui->videosinkComboBox->setCurrentIndex(i);
            break;
        }
    }
}

void configDialog::setRubyExec(QString rbe)
{
    rubyExec = rbe;
    ui->rubyLineEdit->setText(rbe);
}

void configDialog::setToyToolDir(QString Tdir)
{
    toyToolDir = Tdir;
    ui->toyToolEdit->setText(Tdir);
}

void configDialog::setFrameMargin(int m)
{
    frameMargin = m;
    ui->frameMarginSpinBox->setValue(m);
}

void configDialog::setEditFont(QFont f)
{
    editFont = f;
    ui->fontSizespinBox->setValue(f.pointSize());
    for (int i = 0; i < ui->fontComboBox->count(); i++)
    {
        if (ui->fontComboBox->itemText(i) == f.family())
        {
            ui->fontComboBox->setCurrentIndex(i);
            break;
        }
    }
    ui->fontPreviewEdit->setFont(f);
}

void configDialog::on_buttonBox_accepted()
{
    qDebug() <<  "Nyo";
    videoSink = ui->videosinkComboBox->currentText();
    rubyExec = ui->rubyLineEdit->text();
    toyToolDir = ui->toyToolEdit->text();
    frameMargin = ui->frameMarginSpinBox->value();
    replaceMode = (ui->replaceCheckBox->checkState() == Qt::Checked);
    editFont.setFamily(ui->fontComboBox->currentFont().family());
    editFont.setPointSize(ui->fontSizespinBox->value());
    accept();
}

void configDialog::on_rubyChooseButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Choose the ruby executable"), "", "");
    if (!fileName.isEmpty())
    {
        ui->rubyLineEdit->setText(fileName);
    }
}

void configDialog::on_toyToolButton_clicked()
{
    QString fileName = QFileDialog::getExistingDirectory(this, tr("Choose the directory for Toyunda Tools"), "", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (!fileName.isEmpty())
    {
        ui->toyToolEdit->setText(fileName);
    }
}



void configDialog::on_classicCheckBox_stateChanged(int arg1)
{
    if (arg1 == Qt::Checked)
    {
        ui->replaceCheckBox->setChecked(false);
        ui->frameMarginSpinBox->setEnabled(false);
    }
}

void configDialog::on_replaceCheckBox_stateChanged(int arg1)
{
    if (arg1 == Qt::Checked)
    {
        ui->classicCheckBox->setChecked(false);
        ui->frameMarginSpinBox->setEnabled(true);
    }
}

void configDialog::on_listView_clicked(const QModelIndex &index)
{
    ui->stackedWidget->setCurrentIndex(index.row());
}

void configDialog::on_fontComboBox_currentIndexChanged(const QString &arg1)
{
    QFont f = ui->fontPreviewEdit->font();
    f.setFamily(arg1);
    ui->fontPreviewEdit->setFont(f);
}

void configDialog::on_fontSizespinBox_valueChanged(int arg1)
{
    QFont f = ui->fontPreviewEdit->font();
    f.setPointSize(arg1);
    ui->fontPreviewEdit->setFont(f);
}

void configDialog::on_buttonBox_rejected()
{
    reject();
}
