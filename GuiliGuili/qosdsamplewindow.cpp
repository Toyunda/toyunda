#include "qosdsamplewindow.h"
#include <QDebug>
#include <QPaintEvent>

QOSDSampleWindow::QOSDSampleWindow(QWidget *parent) :
    QWidget(parent)
{
    m_toydrawer.setLogo(QImage(":/main/Toyunda logo.png"), 1.0);
    hRatio = 1.0;
    vRatio = 1.0;
}

void QOSDSampleWindow::paintEvent(QPaintEvent *ev)
{
    qDebug() << "Paint event";
    QList<ToyundaSyl> sylMire;
    QList<ToyundaText> textMire;
    ToyundaText t1;
    t1.text = "This is an example of text yyy";
    t1.start = 0;
    t1.stop = 100;
    t1.posx = -1;
    t1.posy = -1;
    t1.size = -1;
    t1.pipeNumber = 0;
    textMire.append(t1);
    t1.pipeNumber = 2;
    textMire.append(t1);
    t1.pipeNumber = 4;
    textMire.append(t1);
    ToyundaSyl s1;
    s1.start = 0;
    s1.posx = -1;
    s1.posy = -1;
    s1.stop = 100;
    s1.length = 2;
    s1.alpha = -1;
    s1.size = -1;
    s1.pos = 0;
    s1.pipeNumber = 1;
    sylMire.append(s1);
    s1.pipeNumber = 3;
    sylMire.append(s1);
    s1.pipeNumber = 5;
    sylMire.append(s1);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setClipRect(ev->rect());
    m_toydrawer.draw(painter, textMire, sylMire);
}

void QOSDSampleWindow::resizeEvent(QResizeEvent *ev)
{
    qDebug() << "resize : " << ev->size();
    QFont f(ToyundaFontName, (float)(ev->size().width()) / ToyundaWidth * (float)(ToyundaFontSize));
    m_toydrawer.setFont(f);
    qDebug() << (float)(ev->size().width()) / (float)ToyundaWidth << (float)(ev->size().height()) / (float)(ToyundaHeight);
    hRatio = (float)(ev->size().width()) / (float)ToyundaWidth;
    vRatio = (float)(ev->size().height()) / (float)(ToyundaHeight);
    m_toydrawer.setRatio((float)(ev->size().width()) / (float)ToyundaWidth, (float)(ev->size().height()) / (float)(ToyundaHeight));
    m_toydrawer.recalcMagic();
    ev->accept();
}

void QOSDSampleWindow::setVRatio(float r)
{
    vRatio = r;
    m_toydrawer.setRatio(hRatio, vRatio);
}

void QOSDSampleWindow::setHRatio(float r)
{
    hRatio = r;
    m_toydrawer.setRatio(hRatio, vRatio);
}

