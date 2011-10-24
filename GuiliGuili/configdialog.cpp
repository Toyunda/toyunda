#include <QFileDialog>
#include "configdialog.h"

ConfigDialog::ConfigDialog()
{
    ui.setupUi(this);
}

void    ConfigDialog::on_selectKaraokeDirButton_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, "Choississez le répertoire du karaoke", "", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    ui.karaokeDirLineEdit->setText(dir);
}
