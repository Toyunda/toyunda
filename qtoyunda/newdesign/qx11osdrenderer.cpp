#include "qx11osdrenderer.h"
#include <QPaintEvent>
#include <X11/extensions/Xrender.h>

QX11OSDRenderer::QX11OSDRenderer(QWidget *parent) : QWidget(parent), ToyundaRenderer()
{
  setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
  setAttribute(Qt::WA_NoSystemBackground, true);
  //setAutoFillBackground(true);
  //  /*QPixmap pix("fond.png");
  //    setMask(pix.mask());*/
  //      //setWindowOpacity(0.1);
  setFixedSize(QSize(ToyundaWidth, ToyundaHeight));
 
  identifier = "qx11osdrenderer";
  QFont f(ToyundaFontName, ToyundaFontSize);
  f.setFixedPitch(true);
  toyundaDrawer.setFont(f);
}

QApplication *QX11OSDRenderer::init(int &ac, char *ag[])
{
  QApplication *toret;
  show();
  return toret;
}

void	QX11OSDRenderer::paintEvent(QPaintEvent *event)
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
  painter.fillRect(rect(), Qt::blue);
  painter.restore();

  toyundaDrawer.draw(painter, toySubStream->getCurrentTextSubtitle(), toySubStream->getCurrentSylSubtitle());
}

void	QX11OSDRenderer::renderUpdate(void)
{
  update();
}
