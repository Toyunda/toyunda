#include "qosd.h"
#include <QPaintEvent>


QOSD::QOSD(QWidget *parent) : QWidget(parent), ToyundaRenderer()
{
  setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
  setAttribute(Qt::WA_NoSystemBackground, true);
  setAttribute(Qt::WA_TranslucentBackground, true);
  //setAutoFillBackground(true);
  //  /*QPixmap pix("fond.png");
  //    setMask(pix.mask());*/
  //      //setWindowOpacity(0.1);
  setFixedSize(QSize(ToyundaWidth, ToyundaHeight));
 
  identifier = "qosd";
  QFont f(ToyundaFontName, ToyundaFontSize);
  f.setFixedPitch(true);
  toyundaDrawer.setFont(f);
}

bool  QOSD::init(QStringList opt)
{
  show();
  return true;
}

void	QOSD::paintEvent(QPaintEvent *event)
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

void	QOSD::renderUpdate(void)
{
  update();
}
