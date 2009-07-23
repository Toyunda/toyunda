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

#ifndef H_FILEPLAYER
#define H_FILEPLAYER

#include <QObject>
#include <QStringList>
#include "scomponent.h"

/** @class FilePlayer
* @brief The class that play the file.
*
* This class handle the video stream. You can use it to play, stop or seek the stream.
*
* To create a FilePlayer just inherit this class and define all methods.
*/
class FilePlayer : public QObject, SComponent
{
	Q_OBJECT;
	public:
		FilePlayer(QObject *parent = 0);
		/**
		 * Call this method first to init the player. The options are specific to
		 * the Player you use, see it documentation.
		 * @param optionList : The list of option the player can need.
                 * @return : true if the initialisation success
		 */
		virtual bool	init(const QStringList optionList = QStringList()) = 0;

	public slots:
		/**
		 * Open the file's name giving in argument, it doesn't start the stream
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

	signals:
		/**
		 * This signal is emit when the stream is stopped
		 */
		void	stopped();
		/**
		 * This signal is emit when the stream is started
		 */
		void	started();
		/**
		 * This signal is emit when the stream is finished
		 */
		void	finished();
		/**
		 * This signal is emit when a new frame is read
		 * @param frame : the number of the frame
		 */
		void	frameChanged(int frame);
};

#endif
