#include "qosd.h"
#include <QPaintEvent>


QOSD::QOSD(QWidget *parent) : QWidget(parent), ToyundaRenderer()
{
  addOption("ratio", 1.0, "Global ratio for ToyundaDrawer, overwrite vratio and hratio");
  addOption("vratio", 1.0, "Vertical ratio for ToyundaDrawer");
  addOption("hratio", 1.0, "Horizontal ratio for ToyundaDrawer");
  setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
  setAttribute(Qt::WA_NoSystemBackground, true);
#if QT_VERSION >= 0x040500
  setAttribute(Qt::WA_TranslucentBackground, true);
#endif
  setIdentifier("qosd");
}

bool  QOSD::init(QStringList opt)
{
  handleOption(opt);
  show();
  toyundaDrawer.setRatio(optionValue["ratio"].toDouble());
  setFixedSize(QSize(toyundaDrawer.width(), toyundaDrawer.height()));
  QFont f(ToyundaFontName, (int) ToyundaFontSize * optionValue["ratio"].toDouble());
  f.setFixedPitch(true);
  toyundaDrawer.setFont(f);
  toyundaDrawer.setLogo(QImage("Toyunda logo.png"));
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
