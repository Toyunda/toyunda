#include "mplayerprofildialog.h"
#include "ui_mplayerprofildialog.h"
#include <QFileDialog>
#include <QDebug>

mplayerProfilDialog::mplayerProfilDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::mplayerProfilDialog)
{
    ui->setupUi(this);
}

mplayerProfilDialog::~mplayerProfilDialog()
{
    delete ui;
}

void mplayerProfilDialog::updateValue()
{
    ui->fontPathLineEdit->setText(mplayerFontPath);
    ui->mplayerExecLineEdit->setText(mplayerExecPath);
    ui->addArgLineEdit->setText(mplayerAdditionnalArg);
    ui->wdLineEdit->setText(mplayerWDPath);
}

void mplayerProfilDialog::on_mplayerExecPathChooserButton_clicked()
{
	mplayerExecPath = QFileDialog::getOpenFileName(this, "Sélectionnez l'executable mplayer-toyunda");
	ui->mplayerExecLineEdit->setText(mplayerExecPath);
}

void mplayerProfilDialog::on_fontPathButton_clicked()
{
	mplayerFontPath = QFileDialog::getOpenFileName(this, "Sélectionnez le fichier font.desc correspondant à la font");
	ui->fontPathLineEdit->setText(mplayerFontPath);
}

void mplayerProfilDialog::on_wdButton_clicked()
{
	mplayerWDPath = QFileDialog::getOpenFileName(this, "Sélectionnez le répertoire où lancer mplayer-toyunda");
	ui->wdLineEdit->setText(mplayerWDPath);
}

void mplayerProfilDialog::on_checkBox_stateChanged(int arg1)
{
	qDebug() << arg1;
}

void mplayerProfilDialog::on_buttonBox_accepted()
{
	mplayerAdditionnalArg = ui->addArgLineEdit->text();
}
