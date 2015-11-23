#ifndef QOSDSAMPLEWINDOW_H
#define QOSDSAMPLEWINDOW_H

#include <QWidget>
#include "../commons/toyundadrawer.h"

class QOSDSampleWindow : public QWidget
{
    Q_OBJECT
public:
    explicit QOSDSampleWindow(QWidget *parent = 0);
    void    paintEvent(QPaintEvent *);
    void    resizeEvent(QResizeEvent *);
    float   vRatio;
    float   hRatio;
    void    setVRatio(float);
    void    setHRatio(float);
    
signals:
    
public slots:
    

private:
    ToyundaDrawer  m_toydrawer;
};

#endif // QOSDSAMPLEWINDOW_H
