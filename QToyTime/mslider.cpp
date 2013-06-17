#include "mslider.h"
#include <QVBoxLayout>

MSlider::MSlider(Qt::Orientation orientation, QWidget *parent) :
    QFrame(parent)
{
    QVBoxLayout* la = new QVBoxLayout(this);

    setLayout(la);
    slider = new QSlider(orientation, this);
    valueLabel = new QLabel(this);
    la->addWidget(valueLabel);
    la->addWidget(slider);

    setFrameStyle(QFrame::Panel | QFrame::Raised);
    setBackgroundRole(QPalette::Background);
    setAutoFillBackground(true);

    connect(slider, SIGNAL(valueChanged(int)), this, SLOT(on_sliderValueChanged(int)));
}

void MSlider::on_sliderValueChanged(int val)
{
    valueLabel->setText(QString::number(val));
}
