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

#ifndef H_OSDT
#define H_OSDT

#include "toystream.h"
#include "toytype.h"
#include <QWidget>

class QToyOSD : public QWidget
{
  Q_OBJECT;
  public:
    QToyOSD(QWidget *parent = 0);
    void	setStream(ToyStream *);

  protected:
    void	paintEvent(QPaintEvent*);

  private:
    ToyStream	*toys;
    int		toyundaWidth;
    int		toyundaHeight;
    QSize	m_size;
    QImage	toylogo;
    float	ratio;
    int		fontSize;
    int		letterw;
    int		letterh;
    int		spacingSize;
};

#endif
