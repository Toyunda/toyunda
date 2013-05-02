#include "gstplayerprofildialog.h"
#include "ui_gstplayerprofildialog.h"

GstPlayerProfilDialog::GstPlayerProfilDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GstPlayerProfilDialog)
{
    ui->setupUi(this);
    fillComboxBox();
}

GstPlayerProfilDialog::~GstPlayerProfilDialog()
{
    delete ui;
}

void GstPlayerProfilDialog::updateValue()
{
    ui->fsCheckBox->setChecked(fullscreen);
    ui->sfscheckBox->setChecked(scaled_fullscreen);
    for (int i = 0; i < ui->audioSinkComboBox->count(); i++)
    {
        if (ui->audioSinkComboBox->itemText(i) == audiosink)
        {
            ui->audioSinkComboBox->setCurrentIndex(i);
            break;
        }
    }
    for (int i = 0; i < ui->videoSinkComboBox->count(); i++)
    {
        if (ui->videoSinkComboBox->itemText(i) == videosink)
        {
            ui->videoSinkComboBox->setCurrentIndex(i);
            break;
        }
    }
}

void GstPlayerProfilDialog::fillComboxBox()
{
    QStringList videosinkList;
    QStringList audiosinkList;

    audiosinkList << "autoaudiosink";
#ifdef Q_WS_X11
    videosinkList << "autovideosink" << "ximagesink" << "xvimagesink" << "glimagesink" << "cluttersink";
    audiosinkList << "alsasink" << "pulsesink";
#endif
#ifdef Q_WS_WIN32
    videosinkList << "directdrawsink" << "d3dvideosink" << "dshowvideosink" << "cluttersink";
    audiosinkList << "directsoundsink";
#endif
    ui->videoSinkComboBox->clear();
    ui->audioSinkComboBox->clear();
    ui->videoSinkComboBox->addItems(videosinkList);
    ui->audioSinkComboBox->addItems(audiosinkList);
}

void GstPlayerProfilDialog::on_fsCheckBox_stateChanged(int arg1)
{
    if (arg1 == Qt::Checked || arg1 == Qt::Unchecked)
        fullscreen = ui->fsCheckBox->isChecked();
}

void GstPlayerProfilDialog::on_sfscheckBox_stateChanged(int arg1)
{
    if (arg1 == Qt::Checked || arg1 == Qt::Unchecked)
        scaled_fullscreen = ui->fsCheckBox->isChecked();
}
