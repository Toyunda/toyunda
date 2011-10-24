#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include <QDialog>
#include "ui_configDialog.h"

class ConfigDialog : public QDialog
{
    Q_OBJECT
    public:
        ConfigDialog();
        Ui::configDialog    ui;
};

#endif // CONFIGDIALOG_H
