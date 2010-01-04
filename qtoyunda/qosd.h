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

#ifndef H_QOSD
#define H_QOSD

#include "toyundarenderer.h"
#include "toyundadrawer.h"
#include <QWidget>

/** @class QOSD
* @brief A Fake OSD toyunda renderer
*
* This render display subs in something that look an overlay
* on top of all application. It's just a QWidget using
* special attribute
* Identifiant : qosd
*/

class QOSD : public QWidget, public ToyundaRenderer
{
  Q_OBJECT;
  public:
    QOSD(QWidget *parent = 0);
    bool  init(QStringList opt = QStringList());

  public slots:
    void	renderUpdate(void);

  protected:
    void	paintEvent(QPaintEvent*);

  private:
    ToyundaDrawer	toyundaDrawer;
};

#endif
