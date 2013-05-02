#include <QColorDialog>
#include "osdprofildialog.h"
#include <QDebug>
#include "ui_osdprofildialog.h"

osdprofildialog::osdprofildialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::osdprofildialog)
{
    ui->setupUi(this);
}

osdprofildialog::~osdprofildialog()
{
    delete ui;
}

void osdprofildialog::on_colorButton_clicked()
{
    QColorDialog    diag;

    if (diag.exec() == QDialog::Accepted)
    {
        default_color = diag.currentColor();
        ui->colorButton->setStyleSheet(QString("background-color: rgb(%1,%2,%3)").arg(diag.currentColor().red()).arg(diag.currentColor().green()).arg(diag.currentColor().blue()));
        qDebug() << QString("background-color: rgb(%1,%2,%3)").arg(diag.currentColor().red()).arg(diag.currentColor().green()).arg(diag.currentColor().blue());
    }

}
