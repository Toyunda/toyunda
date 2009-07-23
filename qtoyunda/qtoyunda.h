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

#ifndef H_QTOYUNDA
#define H_QTOYUNDA

#include <QObject>
#include <QStringList>
#include "scomponent.h"
#include "toyundasub.h"
#include "toyundarenderer.h"
#include "fileplayer.h"
#include "toyundasubstream.h"

/** @class QToyunda
* @brief Class to create a QToyunda
*
* This class take care of all internal thing needed to create a toyunda player.
* You just have to specify the player and the renderer.
*
* Example how to use the class.
*
* \code
* QToyunda toyunda("xine", "qosd");
* toyunda.init();
* toyunda.load("myvideo.avi", "mysubtitle.txt");
* toyunda.play();
* \endcode
*/
class QToyunda : public QObject, SComponent
{
  Q_OBJECT;

  public:
    /**
     * Create a toyunda player
     */
    QToyunda(QStringList arg);

// Accessors
    /**
     * @brief Get the player options.
     * @return the player options.
     */
    QStringList playerOption() const;
    /**
     * @brief Set the player options
     * @param playerOpt : the player options lits
     */
    void  setPlayerOption(QStringList playerOpt);
    /**
     * @brief Get the renderer options.
     * @return the renderer options list
     */
    QStringList rendererOption() const;
    /**
     * @brief Set the renderer options
     * @param rendererOpt : the renderer options list
     */
    void  setRendererOption(QStringList rendererOpt);
    /**
     * @brief Get the name of the player
     * @return the name of the player
     */
    QString playerName() const;
    /**
     * @brief Set the player
     * @param playerNam  : the name of the player
     */
    void  setPlayerName(QString playerNam);
    /**
     * Get the renderer name
     * @return the renderer name
     */
    QString rendererName() const;
    /**
     * Set the renderer name
     * @param rendererNam : the renderer name
     */
    void  setRendererName(QString rendererNam);
    /**
     * Get the video file name
     * @return the video file name
     */
    QString videoFile() const;
    /**
     * Set the video file name
     * @param Video 
     */
    void  setVideoFile(QString videofil);
    /**
     * Get the subtitlte file name
     * @return the subtitle file name
     */
    QString subtitleFile() const;
    /**
     * Set the subtitle file name
     * @param subtitleFil : the subtitle file name
     */
    void  setSubtitleFile(QString subtitleFil);
    /**
     * Load the video and subtitle file
     * @param videofil the video file name
     * @param subtitleFil the subtitle file name
     */
    void  load(QString videofil, QString subtitleFil); 
    /**
     * You must call this to intialise some internal stuff
     */
    void  init();

    /**
     * play the file
     */
    void  play();
  private:
    ToyundaRenderer   *renderer;
    FilePlayer        *player;
    ToyundaSubStream  *toyundaSub;

    QStringList       s_playerOption; /*!< The options of the player */
    QStringList       s_rendererOption; /*!< The options of the renderer */

    QString           s_rendererName; /*!< The name of the renderer */
    QString           s_playerName; /*!< The name of the player */

    QString           s_videoFile; /*!< The name of the video file */
    QString           s_subtitleFile; /*!< The name of the subtitle file */
    void              selectPlayer();
    void              selectRenderer();
};

#endif