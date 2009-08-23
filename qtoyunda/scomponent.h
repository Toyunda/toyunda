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


#ifndef H_SCOMPONENT
#define H_SCOMPONENT
#include "sqarg.h"

class SComponent 
{
  public:
    SComponent();
    void	handleOption(QStringList listArg);
    QString	identifier() const;
    void	showOption();

  protected:
    void	            setIdentifier(const QString id);
    QMap<QString, QVariant> optionValue;
    void		    addOption(QString name, QVariant defaultvalue, QString description, bool mandatory = false);
    QString		    m_identifier;
  private:
    SQArgDescMap	    optionDesc;
};

#endif

