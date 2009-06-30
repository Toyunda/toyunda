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

#ifndef H_QX11OSDRENDER
#define H_QX11OSDRENDER

#include "toyundarender.h"
#include "toyundadrawer.h"
#include <QWidget>

/** @class QX11OSDRemder
* @brief A Fake OSD toyunda render using XRender
*
* This render use x11 xrender extension to draw the subtitle
* on top of all application. It's just a QWidget using
* a special X11 visual
* Identifiant : xrender
*/

class QX11OSDRender : public QWidget, ToyundaRender
{
  Q_OBJECT;
  public:
    QX11OSDRender(QWidget *parent = 0);

  public slot:
    void	renderUpdate(void);

  protected:
    void	paintEvent(QPaintEvent*);

  private:
    ToyundaDrawer	toyundaDrawer;
};

#endif
