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
    QString rubyExec;
    QString toyToolDir;
    void    setVideoSink(QString vsink);
    void    setRubyExec(QString rbe);
    void    setToyToolDir(QString Tdir);
    
private slots:
    void on_buttonBox_accepted();

    void on_rubyChooseButton_clicked();

    void on_toyToolButton_clicked();

private:
    Ui::configDialog *ui;
};

#endif // CONFIGDIALOG_H
