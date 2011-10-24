/*
Copyright (C) 2008 - Sylvain Colinet
This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/


#include "qosd.h"
#include <QPaintEvent>


QOSD::QOSD(QWidget *parent) : ToyundaRenderer(), QWidget(parent)
{
  addOption("ratio", 1.0, "Global ratio for ToyundaDrawer, overwrite vratio and hratio");
  addOption("vratio", 1.0, "Vertical ratio for ToyundaDrawer");
  addOption("hratio", 1.0, "Horizontal ratio for ToyundaDrawer");
  addOption("iratio", 1.0, "Interline ratio for ToyundaDrawer");
  addOption("logoratio", 0.8, "Scaling of the logo, the default size of the logo is the interline sizex0.8, 0.8 is this option");
  addOption("logo", "Toyunda logo.png", "Set the logo file");
  setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::X11BypassWindowManagerHint);
  setAttribute(Qt::WA_NoBackground, true);
  setAttribute(Qt::WA_NoSystemBackground, true);
  setStyleSheet("background:transparent;");
  setAttribute(Qt::WA_TranslucentBackground, true);
  setIdentifier("qosd");
}

bool  QOSD::init(QStringList opt)
{
  handleOption(opt);
  toyundaDrawer.setIntervalRatio(optionValue["iratio"].toDouble());
  toyundaDrawer.setRatio(optionValue["ratio"].toDouble());
  setFixedSize(QSize(toyundaDrawer.width(), toyundaDrawer.height()));
  QFont f(ToyundaFontName, (int) ToyundaFontSize * optionValue["ratio"].toDouble());
  f.setFixedPitch(true);
  toyundaDrawer.setFont(f);
  toyundaDrawer.setLogo(QImage(optionValue["logo"].toString()));
  qDebug() << toyundaDrawer.width() << toyundaDrawer.height();
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

void	QOSD::hide()
{
	QWidget::hide();
}

void	QOSD::show()
{
	QWidget::show();
}


void	QOSD::renderUpdate(void)
{
    update();
}

ToyundaRenderer*    QOSD::getMe()
{
    return this;
}

Q_EXPORT_PLUGIN2(qtoyunda_qosdrenderer, QOSD)