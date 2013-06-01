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

#ifndef NEWTIMEDIALOG_H
#define NEWTIMEDIALOG_H

#include <QDialog>

namespace Ui {
class NewTimeDialog;
}

class NewTimeDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit NewTimeDialog(QWidget *parent = 0);
    ~NewTimeDialog();

    QString lyrFile;
    QString frmFile;
    QString iniFile;
    QString videoFile;
    QString subFile;
    QString baseDir;
    
private slots:
    void on_titleEdit_textEdited(const QString &arg1);

    void on_prefixEdit_textEdited(const QString &arg1);

    void on_videoChooseButton_clicked();

    void on_lyrChooseButton_clicked();

    void on_frmChooseButton_clicked();

    void on_buttonBox_accepted();

private:
    Ui::NewTimeDialog *ui;

    bool    m_lyrChoosed;
    bool    m_frmChoosed;
    bool    m_iniChoosed;
    void    setBaseDir(QString file);
};

#endif // NEWTIMEDIALOG_H
