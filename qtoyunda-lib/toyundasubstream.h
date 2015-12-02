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

#ifndef H_TOYUNDASTREAM
#define H_TOYUNDASTREAM

#include <QString>
#include <QList>
#include <QObject>
#include "toyundasub.h"

/** @class ToyundaSubStream
* @brief The class that handle the subtitle stream
* 
* To create a new parser inherit this class.
* You can just define the createFromFile method and build the subtitle list in the
* allText and allSub variable, ToyundaSubStream already take care of the rest.
*
* YOU NEED TO SET currentSyl and currentText iterator at the end of createFromFile method
* \code
* currentItSyl = allSyl.begin();
*  currentItText = allText.begin();
* \endcode
*
* How this class work :
*
* the createFromFile method create the syl and text file.
* When the setCurrentFrame method is called (eg : connected to the FilePlayer::frameChanged signal)
* the currentText and CurrentSyl is modified to reflect the change. They contain all subtitles that 
* the renderer must show according to current frame number.
*/
class ToyundaSubStream : public QObject
{
  Q_OBJECT
  public:
    ToyundaSubStream();
    /**
     * Construct the subtitle stream from fileName.
     * @param fileName : The name of the file to load
     */
    ToyundaSubStream(QString fileName);

    /**
     * Get the current text subtitles list.
     * @return the current text subtitle list
     */
    virtual QList<ToyundaText>  getCurrentTextSubtitle() const;
    /**
     * Get the current syl subtitles list.
     * @return the current syl subtitles list
     */
    virtual QList<ToyundaSyl>   getCurrentSylSubtitle() const;
    /**
     * Get the number of the current frame.
     * @return the number of the current frame
     */
    int				getCurrentFrame() const;
    /**
     * Get the identifier of the stream.
     * @return the identifier of the stream
     */
    QString			getIdentifier() const;
    /**
     * Use this method to move throw the stream.
     * @param step : the step in millisec
     */
    virtual void		seek(const int step);
    virtual void		reinit() = 0;
    /**
     * Load a file an create the substream from it.
     * @param fileName : the name of the file
     */
    virtual bool		createFromFile(const QString fileName) = 0;

    virtual                     ~ToyundaSubStream();

  public slots:
    /**
     * Set the current frame number.
     * @param frameNumber the current frame number
     */
    virtual void	setCurrentFrame(const int frameNumber);

  signals:
    /**
     * This signal is emit when something change in the current subtitle
     * like new subtitle to display or color changing.
     */
    void	currentSubChanged();

  protected:
    QList<ToyundaText>  currentText; /*!< The current text list */
    QList<ToyundaSyl>   currentSyl; /*!< The current syl list */
    int                 currentFrame; /*!< the number of the current frame */
    QString		identifier; /*!< The identifier of the toyundastream class */
    QVector<ToyundaText> allText; /*!< Contain the whole text subtitles */
    QVector<ToyundaSyl>  allSyl; /*!< Contain the whole syl subtitles */
    QVector<ToyundaText>::iterator currentItText; /*!< The position in the text subtitle list */
    QVector<ToyundaSyl>::iterator currentItSyl; /*!< The position in the syl subtitle list */
};

#endif

