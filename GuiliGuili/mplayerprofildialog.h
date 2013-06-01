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

#ifndef MPLAYERPROFILDIALOG_H
#define MPLAYERPROFILDIALOG_H

#include <QDialog>

namespace Ui {
class mplayerProfilDialog;
}

class mplayerProfilDialog : public QDialog
{
	Q_OBJECT

public:
	explicit mplayerProfilDialog(QWidget *parent = 0);
	~mplayerProfilDialog();
    void            updateValue();
	QString			mplayerExecPath;
	QString			mplayerWDPath;
	QString			mplayerFontPath;
	QString			mplayerAdditionnalArg;
	bool			fullscreen;

private slots:
	void on_mplayerExecPathChooserButton_clicked();

	void on_fontPathButton_clicked();

	void on_wdButton_clicked();

	void on_checkBox_stateChanged(int arg1);

	void on_buttonBox_accepted();

private:
	Ui::mplayerProfilDialog *ui;
    QString m_defDir;
};

#endif // MPLAYERPROFILDIALOG_H
