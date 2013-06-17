#ifndef MSLIDER_H
#define MSLIDER_H

#include <QFrame>
#include <QSlider>
#include <QLabel>

class MSlider : public QFrame
{
    Q_OBJECT
public:
    explicit MSlider(Qt::Orientation orientation = Qt::Horizontal, QWidget *parent = 0);
    QSlider* slider;
    
signals:
    
public slots:
    
private slots:
    void    on_sliderValueChanged(int);
private:
    QLabel*  valueLabel;
};

#endif // MSLIDER_H
