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


#ifndef H_RAWSUBSTREAM
#define H_RAWSUBSTREAM

#include "toyundasubstream.h"
/** @class RawSubStream
* @brief Handle file of pure toyunda sub
*
* This class handle raw toyunda sub file
* The toyunda format look like {start}{stop}{l:option}blahblah
* See ToyundaSubStream documentation for more information
*/
class RawSubStream : public ToyundaSubStream
{
  Q_OBJECT
  private :
  struct GenLineDesc {
	uint	start;
	uint	stop;
	uint	pipeNumber;
	int	posx;
	int	posy;
	int	pos2x;
	int	pos2y;
	int	lenght;
	int	size;
    int size2;
	uint	pos;
	QString	text;
	QColor	color;
	QColor	color2;
  };
 QList<GenLineDesc> parseToyundaLine(QString);
 void	parsePipe(QString &, GenLineDesc &);
 void	parseOption(QString &, GenLineDesc &);

  public :
    RawSubStream();
    RawSubStream(const QString);
    bool	createFromFile(const QString);
    void	reinit();
    ~RawSubStream();
};


#endif
