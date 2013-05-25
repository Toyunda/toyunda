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
