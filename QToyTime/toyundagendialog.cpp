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

#include "toyundagendialog.h"
#include "ui_toyundagendialog.h"
#include <QDebug>

ToyundaGenDialog::ToyundaGenDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ToyundaGenDialog)
{
    ui->setupUi(this);
    m_process = new QProcess;
    connect(m_process, SIGNAL(readyReadStandardError()), this, SLOT(onReadyStdError()));
}

ToyundaGenDialog::~ToyundaGenDialog()
{
    delete ui;
}

void ToyundaGenDialog::execute(QString exec, QStringList args, QString fileoutput, QString wd)
{
    ui->executeLabel->setText(exec + " " + args.join(" ") + " > " + fileoutput);
    ui->textEdit->clear();
    m_process->setWorkingDirectory(wd);
    m_process->setStandardOutputFile(fileoutput);
    m_process->start(exec, args);
    this->exec();
}

void ToyundaGenDialog::onReadyStdError()
{
    ui->textEdit->insertPlainText(m_process->readAllStandardError());
}

void ToyundaGenDialog::on_closeButton_clicked()
{
    m_process->kill();
    accept();
}
