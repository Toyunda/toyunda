#include "osdprofildialog.h"
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
