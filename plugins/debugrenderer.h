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

#ifndef H_DEBUGRENDER
#define H_DEBUGRENDER

#include <QObject>
#include <QTextStream>
#include "toyundarenderer.h"
#include "abstractrenderer.h"

/** @class DebugRenderer
* @brief A debug renderer
*
* This render is designed to debug. It print
* verbose information about sub currently playing
* Indentifiant : debug
*/

class DebugRenderer : public QObject, public ToyundaRenderer, public AbstractRenderer
{
    Q_OBJECT
    Q_INTERFACES(AbstractRenderer)

  public:
    DebugRenderer();
    void    dispose();
    void    hide();
    void    show();
    ToyundaRenderer *getMe();

  public slots:
    void    renderUpdate(void);
};


#endif
