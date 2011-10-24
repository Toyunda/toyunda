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

    private slots:
        void    on_selectKaraokeDirButton_clicked();
};

#endif // CONFIGDIALOG_H
