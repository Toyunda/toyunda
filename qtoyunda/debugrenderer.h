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

#ifndef H_DEBUGRENDER
#define H_DEBUGRENDER

#include <QObject>
#include <QTextStream>
#include "toyundarenderer.h"

/** @class DebugRenderer
* @brief A debug renderer
*
* This render is designed to debug. It print
* verbose information about sub currently playing
* Indentifiant : debug
*/

class DebugRenderer : public QObject, public ToyundaRenderer
{
  Q_OBJECT;
  public:
    DebugRenderer();

  public slots:
    void    renderUpdate(void);
};


#endif
