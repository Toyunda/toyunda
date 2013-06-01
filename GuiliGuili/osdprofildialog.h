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

#ifndef OSDPROFILDIALOG_H
#define OSDPROFILDIALOG_H

#include <QDialog>

namespace Ui {
class osdprofildialog;
}

class osdprofildialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit osdprofildialog(QWidget *parent = 0);
    ~osdprofildialog();

    float           global_ratio;
    QColor         default_color;
    unsigned int    width;
    unsigned int    height;
    float           v_ratio;
    float           h_ratio;
    bool            altered_version;
    QString         font;
    QString         default_toyunda_logo;

    
private slots:
    void on_colorButton_clicked();

private:
    Ui::osdprofildialog *ui;
};

#endif // OSDPROFILDIALOG_H
