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

#ifndef GSTPLAYERPROFILDIALOG_H
#define GSTPLAYERPROFILDIALOG_H

#include <QDialog>

namespace Ui {
class GstPlayerProfilDialog;
}

class GstPlayerProfilDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit GstPlayerProfilDialog(QWidget *parent = 0);
    ~GstPlayerProfilDialog();
    void    updateValue();
    QString videosink;
    QString audiosink;
    bool    fullscreen;
    bool    scaled_fullscreen;
    
private slots:
    void on_fsCheckBox_stateChanged(int arg1);

    void on_sfscheckBox_stateChanged(int arg1);

private:
    Ui::GstPlayerProfilDialog *ui;

    void    fillComboxBox();
};

#endif // GSTPLAYERPROFILDIALOG_H
