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

#ifndef H_QTOYUNDA
#define H_QTOYUNDA

#include <QObject>
#include <QStringList>
#include <QDir>
#include "scomponent.h"
#include "toyundasub.h"
#include "toyundarenderer.h"
#include "fileplayer.h"
#include "toyundasubstream.h"


struct PluginType {
    enum Type {
        Player,
        Renderer
    };
};
struct   PluginInfo
{
    QFileInfo                fileInfo;
    QString                  name;
    QString                  description;
    PluginType::Type         type;
};


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
  Q_OBJECT

  public:
   /**
     * Create a toyunda player with the renderer and the player.
     * @param playerNam : The name of the player
     * @param rendererNam : The name of the renderer
     * @param playerOpt : The option for the player
     * @param rendererOpt : The option for the renderer
     */
    QToyunda();
    QToyunda(QString playerNam, QString rendererNam, 
             QStringList playerOpt = QStringList(),
             QStringList rendererOpt = QStringList());

    // Accessors
    /**
     * @brief Set the plugin path
     * @param pluginDirectory : the plugin path
     */
    void    setPluginDirectory(QDir pluginDirectory);
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
    const QString& playerName() const;
    /**
     * @brief Set the player
     * @param playerNam  : the name of the player
     */
    void  setPlayerName(QString playerNam);
    /**
     * Get the renderer name
     * @return the renderer name
     */
    const QString& rendererName() const;
    /**
     * Set the renderer name
     * @param rendererNam : the renderer name
     */
    void  setRendererName(QString rendererNam);
    /**
     * Get the video file name
     * @return the video file name
     */
    const   QString& videoFile() const;
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
     * Load a init file
     * @param initFile : the .ini file name
     */
    void  load(QString initFile); 
 
    /**
     * You must call this to intialise some internal stuff
     */
    bool  init();
    /**
     * Display the player option
     */
    void  showPlayerOption();
    /**
     * Display the renderer option
     */
    void  showRendererOption();

    /**
     * Load plugins
     * @return true if can find and load plugin
     */
    bool    loadPlugins();

    const QList<PluginInfo>& getPluginInfos() const;
    void    setRendererQWidgetParent(QWidget *);
    void    dispose();
    /**
     * play the file
     */
	void  play();

	void pause();

	void stop();
	
	void	hideRenderer();
	void	showRenderer();


	signals:
		void	stopped();
		void	paused();
		void	played();
		void	finished();
  private slots:
    /**
     * Quit the application
     */
    void  quit();

  private:
    ToyundaRenderer   *renderer;
    FilePlayer        *player;
    ToyundaSubStream  *toyundaSub;

    QDir              s_pluginDirectory;

    QStringList       s_playerOption; /*!< The options of the player */
    QStringList       s_rendererOption; /*!< The options of the renderer */

    QString           s_rendererName; /*!< The name of the renderer */
    QString           s_playerName; /*!< The name of the player */

    QString           s_videoFile; /*!< The name of the video file */
    QString           s_subtitleFile; /*!< The name of the subtitle file */
    bool              selectPlayer();
    bool              selectRenderer();
    QList<QObject *>            s_pluginList;
    QList<FilePlayer *>         s_filePlayerPlugins;
    QList<ToyundaRenderer *>    s_toyundaRendererPlugins;
    QList<PluginInfo>           s_pluginInfos;
};

#endif
