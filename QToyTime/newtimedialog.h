#ifndef NEWTIMEDIALOG_H
#define NEWTIMEDIALOG_H

#include <QDialog>

namespace Ui {
class NewTimeDialog;
}

class NewTimeDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit NewTimeDialog(QWidget *parent = 0);
    ~NewTimeDialog();

    QString lyrFile;
    QString frmFile;
    QString iniFile;
    QString videoFile;
    QString subFile;
    QString baseDir;
    
private slots:
    void on_titleEdit_textEdited(const QString &arg1);

    void on_prefixEdit_textEdited(const QString &arg1);

    void on_videoChooseButton_clicked();

    void on_lyrChooseButton_clicked();

    void on_frmChooseButton_clicked();

    void on_buttonBox_accepted();

private:
    Ui::NewTimeDialog *ui;

    bool    m_lyrChoosed;
    bool    m_frmChoosed;
    bool    m_iniChoosed;
    void    setBaseDir(QString file);
};

#endif // NEWTIMEDIALOG_H
