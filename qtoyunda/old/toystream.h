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

#ifndef H_TOYSTREAM
#define H_TOYSTREAM

#include <set>
#include "toytype.h"
#include <QString>
#include <QList>
#include <QObject>

class ToyStream : public QObject
{
  Q_OBJECT;
  public:
    ToyStream();

    void		loadFile(QString);
    QList<ToyText>	getCurrentText();
    QList<ToySyl>	getCurrentSyl();
    QColor		toycolor2QColor(QString);

  public slots:
    void	setCurrentFrame(int);
 
  signals:
    void	subChanged();

  private:
    QList<ToyText> currentText;
    QList<ToySyl> currentSyl;
    int		currentframe;
    std::set<ToyText> text;
    std::set<ToySyl> syl;

};

#endif
