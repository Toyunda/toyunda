#include "newtimedialog.h"
#include "ui_newtimedialog.h"
#include <QFileDialog>

NewTimeDialog::NewTimeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewTimeDialog)
{
    ui->setupUi(this);
}

NewTimeDialog::~NewTimeDialog()
{
    delete ui;
}

void NewTimeDialog::on_titleEdit_textEdited(const QString &arg1)
{
    if (!m_frmChoosed)
        ui->frmEdit->setText(arg1 + ".frm");
    if (!m_lyrChoosed)
        ui->lyrEdit->setText(arg1 + ".lyr");
    if (!m_iniChoosed)
        ui->fileNameEdit->setText(ui->prefixEdit->text() + " - " + arg1 + ".ini");
    ui->subtitleEdit->setText(arg1 + ".txt");
}

void NewTimeDialog::on_prefixEdit_textEdited(const QString &arg1)
{
    if (!m_iniChoosed)
        ui->fileNameEdit->setText(arg1 + " - " + ui->titleEdit->text() + ".ini");
}

void NewTimeDialog::on_videoChooseButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Choose video"),
                                                    "", "");
    if (!fileName.isNull())
    {
        ui->videoEdit->setText(fileName);
        setBaseDir(fileName);
    }
}

void NewTimeDialog::on_lyrChooseButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Choose lyr file"), "", "Lyrics file (*.lyr)");
    if (!fileName.isNull())
    {
        ui->lyrEdit->setText(fileName);
        m_lyrChoosed = true;
    }
}

void NewTimeDialog::on_frmChooseButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                            tr("Choose FRM file"), "", "FRM file (*.frm)");
    if (!fileName.isNull())
    {
        ui->frmEdit->setText(fileName);
        m_frmChoosed = true;
    }
}

void NewTimeDialog::on_buttonBox_accepted()
{
    lyrFile = ui->lyrEdit->text();
    frmFile = ui->frmEdit->text();
    videoFile = ui->videoEdit->text();
    iniFile = ui->fileNameEdit->text();
    subFile = ui->subtitleEdit->text();
}

void NewTimeDialog::setBaseDir(QString file)
{
    QFileInfo   fi(file);

    baseDir = fi.path();
}
