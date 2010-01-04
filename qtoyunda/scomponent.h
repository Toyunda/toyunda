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

/** @class SComponent
* @brief Base class of all component
*
* This class define method to handle option or identifier
*
* For option handling :
* In the constructor of your class call addOption method to define the option you want to take
* Later call handleOption with a list of QString formed like this option=value, optionValue will be fill
* with the value of argument
* Example :
* \code
* addOption("cakesavor", "chocolat", "The savor of the cake, default chocolat");
* addOption("filesize", 512, "the size of the file [default : 512]");
* QStringList l << "cakesavor=honey" << "filesize=640";
* handleOption(l);
* qDebug() << optionValue["cakesavor"].toString(); // display honey
* qDebug() << optionValue["filesize"].toInt(); // display 640
* \endcode
*/

class SComponent 
{
  public:
    SComponent();
    /**
     * This method take list of strings like this "arg=value" to fill the optionValue attribut
     * @param listArg the list of option
     */
    void	handleOption(QStringList listArg);
    /**
     * Return the identifier of the component
     * @return the identifier of the component
     */
    QString	identifier() const;
    /**
     * Show the avalaible option define by the component
     */
    void	showOption();

  protected:
    /**
     * Set the identifier of the component
     * @param id the identifier
     */
    void	            setIdentifier(const QString id);
    QMap<QString, QVariant> optionValue;
    void		    addOption(QString name, QVariant defaultvalue, QString description, bool mandatory = false);
    QString		    m_identifier;
  private:
    SQArgDescMap	    optionDesc;
};

#endif

