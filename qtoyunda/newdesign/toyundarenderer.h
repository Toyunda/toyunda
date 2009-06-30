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


#ifndef H_TOYUNDA_RENDER
#define H_TOYUNDA_RENDER

#include <QApplication>
#include <QWidget>
#include "toyundasubstream.h"


/** @class ToyundaRenderer
* @brief The base class of all renderer
*
* For creating Toyunda renderer you must inherit this class
* If you creating a renderer using Qt you can use the ToyundaDrawer class
* to draw on your QPainter.
*/

class ToyundaRenderer : public QObject
{
  Q_OBJECT;
  public:
        /** 
        * @brief define this function if you need special QApplication construct
        * 
        * Some renderer need to overwrite this to init some internal thing in Qt
        * @param ac : argument count
        * @param ag : arguments
	* @return : The QApplication
	*/
    virtual QApplication    *init(int ac, char *ag[]);

	/** 
	* @brief Set the Toyunda Stream to work on it
	* 
	* @param toyStream : the toyundastream
	*/
    void setToyundaSubStream(ToyundaSubStream *toyStream);

    /**
    * @brief Get the identifier of the renderer
    *
    */
    QString getIdentifier() const;

  public slots:
    /**
     * this function tell the render to update the display of subtitle
     */
    virtual void    renderUpdate() = 0;

  protected:
    ToyundaRenderer();
    QString             identifier; /*!< The name of the renderer */
    ToyundaSubStream    *toySubStream; /*!< The Toyunda subtitle Stream */
};

#endif
