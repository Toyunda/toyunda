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

#ifndef H_QOSD
#define H_QOSD

#include "toyundarenderer.h"
#include "toyundadrawer.h"
#include "../abstractrenderer.h"
#include <QWidget>

/** @class QOSD
* @brief A Fake OSD toyunda renderer
*
* This render display subs in something that look an overlay
* on top of all application. It's just a QWidget using
* special attribute
* Identifiant : qosd
*/

class QOSD : public QWidget, public ToyundaRenderer, public AbstractRenderer
{
  Q_OBJECT
    Q_INTERFACES(AbstractRenderer)
  public:
    QOSD(QWidget *parent = 0);
    bool  init(QStringList opt = QStringList());
    void    dispose();
    void	show();
    void	hide();
    void        setQWidgetParent(QWidget *);
    ToyundaRenderer *getMe();

  public slots:
    void	renderUpdate(void);

  protected:
    void	paintEvent(QPaintEvent*);

  private:
    ToyundaDrawer	toyundaDrawer;
};

#endif
