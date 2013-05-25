#include "mplayerprofildialog.h"
#include "ui_mplayerprofildialog.h"
#include <QFileDialog>
#include <QDebug>

mplayerProfilDialog::mplayerProfilDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::mplayerProfilDialog)
{
    ui->setupUi(this);
    m_defDir = qApp->applicationDirPath();
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
    ui->checkBox->setChecked(fullscreen);
}

void mplayerProfilDialog::on_mplayerExecPathChooserButton_clicked()
{
    mplayerExecPath = QFileDialog::getOpenFileName(this, "Sélectionnez l'executable mplayer-toyunda", m_defDir);
    m_defDir = QFileInfo(mplayerExecPath).absoluteFilePath();
	ui->mplayerExecLineEdit->setText(mplayerExecPath);
}

void mplayerProfilDialog::on_fontPathButton_clicked()
{
    mplayerFontPath = QFileDialog::getOpenFileName(this, "Sélectionnez le fichier font.desc correspondant à la font", m_defDir);
	ui->fontPathLineEdit->setText(mplayerFontPath);
}

void mplayerProfilDialog::on_wdButton_clicked()
{
    mplayerWDPath = QFileDialog::getExistingDirectory(this, "Choississez le répertoire d'execution de mplayer", m_defDir, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	ui->wdLineEdit->setText(mplayerWDPath);
}

void mplayerProfilDialog::on_checkBox_stateChanged(int arg1)
{
    if (arg1 == Qt::Checked || arg1 == Qt::Unchecked)
        fullscreen = ui->checkBox->isChecked();
}

void mplayerProfilDialog::on_buttonBox_accepted()
{
	mplayerAdditionnalArg = ui->addArgLineEdit->text();
}
