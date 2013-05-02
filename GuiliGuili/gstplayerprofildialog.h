#ifndef GSTPLAYERPROFILDIALOG_H
#define GSTPLAYERPROFILDIALOG_H

#include <QDialog>

namespace Ui {
class GstPlayerProfilDialog;
}

class GstPlayerProfilDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit GstPlayerProfilDialog(QWidget *parent = 0);
    ~GstPlayerProfilDialog();
    void    updateValue();
    QString videosink;
    QString audiosink;
    bool    fullscreen;
    bool    scaled_fullscreen;
    
private slots:
    void on_fsCheckBox_stateChanged(int arg1);

    void on_sfscheckBox_stateChanged(int arg1);

private:
    Ui::GstPlayerProfilDialog *ui;

    void    fillComboxBox();
};

#endif // GSTPLAYERPROFILDIALOG_H
