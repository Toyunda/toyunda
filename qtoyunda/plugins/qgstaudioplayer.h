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

#ifndef H_QGSTAUDIOPLAYER
#define H_QGSTAUDIOPLAYER

#include "../fileplayer.h"
#include "../abstractfileplayer.h"
#include <QGst/Pipeline>
#include <QGst/Element>
#include <QGst/Bin>
#include <QGst/Message>
#include <QGst/Pad>
#include <QGst/Caps>

class QGstAudioPlayer : public FilePlayer, public AbstractFilePlayer
{
        Q_OBJECT
    Q_INTERFACES(AbstractFilePlayer)
	public:
		QGstAudioPlayer();
		bool	init(const QStringList optionList = QStringList());
                FilePlayer *getMe();

	public slots:
		void	open(const QString fileName);
		void	play();
		void	stop();
		void	seek(const int step);


	private slots:
		void	checkFrame();
	private:
		void			new_decoded_pad(const QGst::PadPtr &, const int);
		bool			autoplug_continue(const QGst::PadPtr &, const QGst::CapsPtr &);
		void    		onBusMessage(const QGst::MessagePtr &message);
		QGst::PipelinePtr	m_pipeline;
		QGst::ElementPtr	m_src;
		QGst::ElementPtr	m_dec;
		QGst::ElementPtr	m_vsink;
		QGst::BinPtr		m_audiobin;
		QGst::BinPtr		m_videobin;
		int			m_framenb;
		double			framerate;

};

QDebug  operator<<(QDebug dbg, const QGst::State &st);

#endif