#ifndef OSDPROFILDIALOG_H
#define OSDPROFILDIALOG_H

#include <QDialog>

namespace Ui {
class osdprofildialog;
}

class osdprofildialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit osdprofildialog(QWidget *parent = 0);
    ~osdprofildialog();

    float           global_ratio;
    QString         default_color;
    unsigned int    width;
    unsigned int    height;
    float           v_ratio;
    float           h_ratio;
    bool            altered_version;
    QString         font;
    QString         default_toyunda_logo;

    
private:
    Ui::osdprofildialog *ui;
};

#endif // OSDPROFILDIALOG_H
