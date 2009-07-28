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

#ifndef H_SQARG
#define H_SQARG
#include <QMap>
#include <QVariant>
#include <QStringList>

struct SQOpt
{
  SQOpt(QString f, QString s, QVariant def, QString desc, QString l, bool m = true, bool sk = false);
  SQOpt();
  QString	fullName;
  QString	shortName;
  QVariant	defaultValue;
  QString	description;
  QString	longHelp;
  bool		mandatory;
  bool		skipAll;
  bool		isNull() const;
  private:
    bool s_null;
};

typedef QMap<QString, SQOpt> SQArgDescMap;

namespace SQArg
{
  bool	fillWithDesc(QMap<QString, QVariant> &toFill, QStringList listArg, SQArgDescMap desc);
  bool	findByShortName(QString optName, SQArgDescMap desc, SQOpt &opt);
  void	generateShortHelp(SQArgDescMap desc);
  void	generateLongHelp(SQArgDescMap desc);
};


#endif
