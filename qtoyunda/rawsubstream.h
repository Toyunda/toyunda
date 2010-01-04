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
#ifndef H_RAWSUBSTREAM
#define H_RAWSUBSTREAM

#include "toyundasubstream.h"
/** @class RawSubStream
* @brief Handle file of pure toyunda sub
*
* This class handle raw toyunda sub file
* The toyunda format look like {start}{stop}{l:option}
* See ToyundaSubStream documentation for more information
*/
class RawSubStream : public ToyundaSubStream
{
  Q_OBJECT;
  public :
    RawSubStream();
    RawSubStream(const QString);
    void	createFromFile(const QString);
    void	handleToyundaOption(QString &toh, ToyundaText &text);
};


#endif
