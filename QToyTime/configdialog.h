#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include <QDialog>

namespace Ui {
class configDialog;
}

class configDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit configDialog(QWidget *parent = 0);
    ~configDialog();
    QString videoSink;
    void    setVideoSink(QString vsink);
    
private slots:
    void on_buttonBox_accepted();

private:
    Ui::configDialog *ui;
};

#endif // CONFIGDIALOG_H
