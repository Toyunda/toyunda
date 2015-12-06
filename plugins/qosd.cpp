/*
    This file is part of qtoyunda software
    Copyright (C) 2008  Sylvain "Skarsnik" Colinet <scolinet@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "qosd.h"
#include <QPaintEvent>
#include <QThread>

#ifdef Q_WS_X11
#include <QX11Info>
#include <X11/extensions/Xfixes.h>
#include <X11/extensions/shape.h>
#endif

QOSD::QOSD(QWidget *parent) : ToyundaRenderer(), QWidget(parent)
{
  addOption("ratio", 1.0, "Global ratio for ToyundaDrawer, overwrite vratio and hratio");
  addOption("vratio", 1.0, "Vertical ratio for ToyundaDrawer");
  addOption("hratio", 1.0, "Horizontal ratio for ToyundaDrawer");
  addOption("iratio", 1.0, "Interline ratio for ToyundaDrawer");
  addOption("logoratio", 0.8, "Scaling of the logo, the default size of the logo is the interline sizex0.8, 0.8 is this option");
  addOption("logo", "Toyunda logo.png", "Set the logo file");
  addOption("font", ToyundaFontName, "The font decription used to render subtitle, default : " + ToyundaFontName);
  addOption("fontsize", ToyundaFontSize, "The font size, default : " + ToyundaFontSize);
  addOption("backgroundcolor", "none", "background color");
  setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::X11BypassWindowManagerHint);
  setWindowTitle("QToyunda Overlay");
  setIdentifier("qosd");
}

bool  QOSD::init(QStringList opt)
{
  handleOption(opt);
  QFont f(optionValue["font"].toString(), (int) optionValue["fontsize"].toDouble() * optionValue["ratio"].toDouble());
  f.setFixedPitch(true);
  if (optionValue["backgroundcolor"].toString() == "none")
  {
      setAttribute(Qt::WA_NoBackground, true);
      setAttribute(Qt::WA_NoSystemBackground, true);
      setStyleSheet("background:transparent;");
      setAttribute(Qt::WA_TranslucentBackground, true);
  }
  else
  {
      setStyleSheet("background-color:" + optionValue["backgroundcolor"].toString() + ";");
  }
#ifdef Q_WS_X11
  if (QX11Info::isCompositingManagerRunning() == false)
  {
      emit error(SQError(SQError::Critical, "You are not running a X11 composite manager, QOSD can't work without one"));
      return false;
  }
#endif
  toyundaDrawer.setFont(f);
  toyundaDrawer.setIntervalRatio(optionValue["iratio"].toDouble());
  toyundaDrawer.setRatio(optionValue["ratio"].toDouble());
  if (optionValue["ratio"].toDouble() == 1.0)
      toyundaDrawer.setRatio(optionValue["hratio"].toDouble(), optionValue["vratio"].toDouble());
  toyundaDrawer.setLogo(QImage(optionValue["logo"].toString()), optionValue["logoratio"].toDouble());
  toyundaDrawer.recalcMagic();
  qDebug() << toyundaDrawer.width() << "x" << toyundaDrawer.height();
  setFixedSize(QSize(toyundaDrawer.width(), toyundaDrawer.height()));

  //set the input region for the window as Null, should let the windows pass event
#ifdef Q_WS_X11
Display *dpy = QX11Info::display();
Window w_id = QWidget::winId();
qDebug() << "Win ID : " << w_id;
XserverRegion region = XFixesCreateRegion (dpy, NULL, 0);
XFixesSetWindowShapeRegion(dpy, w_id, ShapeInput, 0, 0, region);
XFixesDestroyRegion (dpy, region);
#endif

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

void QOSD::setQWidgetParent(QWidget *qobj)
{
    setParent(qobj);
}

void QOSD::dispose()
{
    close();
}
