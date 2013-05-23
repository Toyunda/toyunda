#include "configdialog.h"
#include "ui_configdialog.h"

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

void configDialog::on_buttonBox_accepted()
{
    videoSink = ui->videosinkComboBox->currentText();
}
