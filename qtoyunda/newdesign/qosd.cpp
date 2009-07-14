#include "qosd.h"
#include <QPaintEvent>


QOSD::QOSD(QWidget *parent) : QWidget(parent), ToyundaRenderer()
{
  setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
  setAttribute(Qt::WA_NoSystemBackground, true);
#if QT_VERSION >= 0x040500
  setAttribute(Qt::WA_TranslucentBackground, true);
#endif
  setFixedSize(QSize(ToyundaWidth, ToyundaHeight));

  s_identifier = "qosd";
  QFont f(ToyundaFontName, ToyundaFontSize);
  f.setFixedPitch(true);
  toyundaDrawer.setFont(f);
  toyundaDrawer.setLogo(QImage("Toyunda logo.png"));
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
