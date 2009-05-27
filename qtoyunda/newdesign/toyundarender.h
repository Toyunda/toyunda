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
#include "toyundadrawer.h"
#include "toyundasubstream.h"


/** @class ToyundaRender
* @brief Define the render to paint the toyunda sub in
*
* For creating Toyunda renderer you must inherit this class
* You only need to set you render specification, to draw the sub
* you just need to call the paint function of the ToyundaPainter
* by passing a QPainter pointer see ToyundaPainter documentation
* for more informations.
*/

class ToyundaRender : public QObject
{
  Q_OBJECT;
  public:
        /** 
        * @brief definne this function if you need special QApplication construct
        * 
        * Some renderer need to overwrite this to init some internal thing in Qt
        * @param app : the QApplication reference
        * @param ac : argument count
        * @param ag : argument
	*/
    virtual void    init(QApplication &app);

	/** 
	* @brief Set the stream to work on it
	* 
	* @param toyStream : the toyundastream
	*/
    void setToyundaSubStream(ToyundaSubStream *toyStream);

    /**
    * @brief Get the identifier of the renderer
    *
    */
    QString getIdentifier();

  public slots:
    virtual void    renderUpdate(void) = 0;

  protected:
    ToyundaRender();
    QString             identifier; /*!< The name of the renderer */
    ToyundaSubStream    *toySubStream; /*!< The Toyunda subtitle Stream */
    ToyundaDrawer       toyDrawer; /*!< The Toyunda drawer */
};

#endif
