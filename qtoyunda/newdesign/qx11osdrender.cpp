#include "qx11osdrender.h"


QX11Render::QX11Render(QWidget *parent) : QWidget(parent), ToyundaRender()
{
  setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
  setAttribute(Qt::WA_NoSystemBackground, true);
  //setAutoFillBackground(true);
  //  /*QPixmap pix("fond.png");
  //    setMask(pix.mask());*/
  //      //setWindowOpacity(0.1);
  setFixedSize(QSize(ToyundaWidth, ToyundaHeight));
}

QX11Render::init()
{

}

QX11Render::paintEvent(QPaintEvent *event)
{
  QPainter painter(this);
  // Magic from QtLabs.
  painter.setRenderHint(QPainter::Antialiasing);
  painter.setClipRect(event->rect());

  //make sure you clean your widget with a transparent
  //  color before doing any rendering
  //  note the usage of a composition mode Source
  //    it's important!
  painter.save();
  painter.setCompositionMode(QPainter::CompositionMode_Source);
  painter.fillRect(rect(), Qt::transparent);
  painter.restore();

  toyundaDrawer.draw(painter, toySubStream->getCurrentTextSubtitle(), toySubStream->getCurrentSylSubtitle());
}

QX11Render::renderUpdate(void)
{
  update();
}
