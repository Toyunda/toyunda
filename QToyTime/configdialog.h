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

#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include <QDialog>
#include <QModelIndex>

namespace Ui {
class configDialog;
}

class configDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit configDialog(QWidget *parent = 0);
    ~configDialog();
    QString videoSink;
    QString rubyExec;
    QString toyToolDir;
    int     frameMargin;
    bool    replaceMode;
    QFont   editFont;

    void    setReplaceMode(bool r);
    void    setVideoSink(QString vsink);
    void    setRubyExec(QString rbe);
    void    setToyToolDir(QString Tdir);
    void    setFrameMargin(int m);
    void    setEditFont(QFont f);
    
private slots:
    void on_buttonBox_accepted();

    void on_rubyChooseButton_clicked();

    void on_toyToolButton_clicked();

    void on_classicCheckBox_stateChanged(int arg1);

    void on_replaceCheckBox_stateChanged(int arg1);

    void on_listView_clicked(const QModelIndex &index);

    void on_fontComboBox_currentIndexChanged(const QString &arg1);

    void on_fontSizespinBox_valueChanged(int arg1);

    void on_buttonBox_rejected();

private:
    Ui::configDialog *ui;
};

#endif // CONFIGDIALOG_H
