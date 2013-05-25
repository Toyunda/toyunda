#include "configdialog.h"
#include "ui_configdialog.h"
#include <QFileDialog>

configDialog::configDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::configDialog)
{
    ui->setupUi(this);
#ifdef  Q_WS_WIN32
    ui->videosinkComboBox->addItem("directdrawsink");
    ui->videosinkComboBox->addItem("d3dvideosink");
    ui->videosinkComboBox->addItem("dshowvideosink");
#endif
#ifdef  Q_WS_X11
    ui->videosinkComboBox->addItem("autovideosink");
    ui->videosinkComboBox->addItem("xvimagesink");
    ui->videosinkComboBox->addItem("ximagesink");
    ui->videosinkComboBox->addItem("glimagesink");
#endif
    ui->videosinkComboBox->addItem("cluttersink");
}

configDialog::~configDialog()
{
    delete ui;
}

void    configDialog::setVideoSink(QString vsink)
{
    for (int i = 0; i < ui->videosinkComboBox->count(); i++)
    {
        if (ui->videosinkComboBox->itemText(i) == vsink)
        {
            ui->videosinkComboBox->setCurrentIndex(i);
            break;
        }
    }
}

void configDialog::setRubyExec(QString rbe)
{
    rubyExec = rbe;
    ui->rubyLineEdit->setText(rbe);
}

void configDialog::setToyToolDir(QString Tdir)
{
    toyToolDir = Tdir;
    ui->toyToolEdit->setText(Tdir);
}

void configDialog::on_buttonBox_accepted()
{
    videoSink = ui->videosinkComboBox->currentText();
    rubyExec = ui->rubyLineEdit->text();
    toyToolDir = ui->toyToolEdit->text();
}

void configDialog::on_rubyChooseButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Choose the ruby executable"), "", "");
    if (!fileName.isEmpty())
    {
        ui->rubyLineEdit->setText(fileName);
    }
}

void configDialog::on_toyToolButton_clicked()
{
    QString fileName = QFileDialog::getExistingDirectory(this, tr("Choose the directory for Toyunda Tools"), "", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (!fileName.isEmpty())
    {
        ui->toyToolEdit->setText(fileName);
    }
}
