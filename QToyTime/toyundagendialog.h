#ifndef TOYUNDAGENDIALOG_H
#define TOYUNDAGENDIALOG_H

#include <QDialog>
#include <QProcess>

namespace Ui {
class ToyundaGenDialog;
}

class ToyundaGenDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit ToyundaGenDialog(QWidget *parent = 0);
    ~ToyundaGenDialog();
    void    execute(QString exec, QStringList args, QString fileoutput, QString wd);

private slots:
    void    onReadyStdError();

    void on_closeButton_clicked();

private:
    Ui::ToyundaGenDialog *ui;

    QProcess*    m_process;
};

#endif // TOYUNDAGENDIALOG_H
