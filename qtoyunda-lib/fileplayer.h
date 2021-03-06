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

#ifndef H_FILEPLAYER
#define H_FILEPLAYER

#include <QObject>
#include <QStringList>
#include "scomponent.h"
#include "sqerror.h"

/** @class FilePlayer
* @brief The class that play the file.
*
* This class handle the video stream. You can use it to play, stop or seek the stream.
*
* To create a FilePlayer just inherit this class and define all methods.
*/
class FilePlayer : public QObject, public SComponent
{
        Q_OBJECT
	public:
                FilePlayer() {;}
		/**
		 * Call this method first to init the player. The options are specific to
		 * the Player you use, see it documentation.
		 * @param optionList : The list of option the player can need.
                 * @return : true if the initialisation success
		 */
		virtual bool	init(const QStringList optionList = QStringList()) = 0;

                /**
                 * Call this method first to dispose the player. must be call only once
                 */
                virtual void    dispose() = 0;

                /**
                 * Return true if the player support Volume Control
                 */
                virtual bool    hasVolumeControl() = 0;

                /**
                 * Used by the plugin loader to get the fileplayer object
                 * It's a limitation of Qt plugin system, the interface exposed to pluginload class
                 * can't inherite QObject so we can't connect signal/slot, see Qt plugin documentation
                 */
                virtual FilePlayer*     getMe() = 0;

	public slots:
		/**
                 * Open the file name, it doesn't start the stream
		 * @param fileName : The name of the file
		 */
		virtual void	open(const QString fileName) = 0;
		/**
		 * Play the file stream
		 */
		virtual void	play() = 0;
		/**
		 * Stop the file stream
		 */
		virtual void	stop() = 0;
		/**
		 * Use this fonction to move on the stream giving by the step argument.
		 * It can be negative to go back in the stream
		 * @param step : the step in millisec
		 */
		virtual void	seek(const int step) = 0;

                /**
                 * Set the volume
                 */
                virtual void    setVolume(uint) = 0;
	signals:
		/**
		 * This signal is emit when the stream is stopped
		 */
		void	stopped();
		/**
		 * This signal is emit when the stream is started
		 */
		void	played();
		/**
		 * This signal is emit when the stream is finished
		 */
		void	finished();
		/**
		 * This signal is emit when the stream is paused
		 */
		void	paused();
		/**
		 * This signal is emit when a new frame is read
		 * @param frame : the number of the frame
		 */
		void	frameChanged(int frame);

                /**
                  * Thie signal is emited when a error occur
                  * @param err : The error
                  */
                void    error(SQError err);
};

#endif
