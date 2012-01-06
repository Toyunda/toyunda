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

#include <QDebug>
#include <QApplication>
#include <QTimer>
#include <QThread>
#include <QFileInfo>
#include "qgstaudioplayer.h"
#include <qplugin.h>
#include <QGst/Init>
#include <QGst/ElementFactory>
#include <QGst/Pad>
#include <QGst/GhostPad>
#include <QGst/Structure>
#include <QGst/Caps>
#include <QGst/Query>
#include <QGst/Bus>
#include <QGst/ClockTime>
#include <QGlib/Connect>
#include <QGlib/Error>
#include <QGst/Fraction>
#include <QGlib/Error>

QGstAudioPlayer::QGstAudioPlayer()
{
        setIdentifier("qgstaudio");
	m_timer = new QTimer(this);
}


void	QGstAudioPlayer::new_decoded_pad(const QGst::PadPtr &pad, const int gbool) 
{
	QGst::PadPtr	audiopad;
	QGst::PadPtr	videopad;
	QGst::CapsPtr	caps;
	QGst::StructurePtr str;
	
	audiopad = m_audiobin->getStaticPad("sink");
	videopad = m_videobin->getStaticPad("sink");
	caps = pad->caps();
	str = caps->internalStructure(0);
	if (str->name().contains("audio"))
	{
	  qDebug() << "Audio linked";
	  pad->link(audiopad);
	}
	if (str->name().contains("video"))
	{
	  qDebug() << "Video linked";
	   pad->link(videopad);
	}
}

bool	QGstAudioPlayer::autoplug_continue(const QGst::PadPtr &pad, const QGst::CapsPtr &cap)
{
	QGst::StructurePtr str = cap->internalStructure(0);
        qDebug() << "Name : " << pad->name() << "Cap :" << cap;
	if (str->hasField("framerate"))
	{
	   QGst::Fraction frac = str->value("framerate").get<QGst::Fraction>();
	   qDebug() << "Framerate : " << frac.numerator << "/" << frac.denominator;
	   framerate = ((double) frac.numerator / frac.denominator);
	   qDebug() << framerate;
	}
	
	return true;
}

void	QGstAudioPlayer::onBusMessage(const QGst::MessagePtr &message)
{
    switch (message->type()) {
    case QGst::MessageEos: //End of stream. We reached the end of the file.
	m_pipeline->setState(QGst::StateNull);
	stop();
	emit finished();
        break;
    case QGst::MessageError: //Some error occurred.
        emit error(SQError(SQError::Critical, "GStreamer error", message.staticCast<QGst::ErrorMessage>()->error().message()));
        stop();
        break;
    case QGst::MessageStateChanged: //The element in message->source() has changed state
	//qDebug() << "changing state : " << message->source()->name() << " - State : " <<
	message.staticCast<QGst::StateChangedMessage>()->newState();
        if ((message->source() == m_pipeline) && (message.staticCast<QGst::StateChangedMessage>()->newState() == QGst::StatePlaying))
        {
		emit played();
                m_timer->start(30);
        }
        if ((message->source() == m_pipeline) && (message.staticCast<QGst::StateChangedMessage>()->newState() == QGst::StatePaused))
		emit paused();
        if ((message->source() == m_pipeline) && (message.staticCast<QGst::StateChangedMessage>()->newState() == QGst::StateNull))
		emit stopped();
        break;
    default:
        break;
    }
}

bool	QGstAudioPlayer::init(const QStringList opt)
{
	handleOption(opt);
	connect(m_timer, SIGNAL(timeout()), this, SLOT(checkFrame()));
#ifdef Q_WS_WIN
        QString tmpstr("GST_PLUGIN_PATH=" + qApp->applicationDirPath().toLatin1() + "/gst-plugins/");
        qDebug() << "Plugin path is : " << tmpstr;
        putenv(tmpstr.toLatin1().constData());
        putenv("GST_DEBUG=*:1");
#endif
        try {
            QGst::init();
        } catch (QGlib::Error qgerr)
        {
            emit error(SQError(SQError::Critical, "Can't initialise QtGstreamer : ", qgerr.message()));
            return false;
        }

	QGst::BusPtr bus;
        QGst::ElementPtr conv, asink, queuea, queuev, resample;

	m_pipeline = QGst::Pipeline::create();
	m_src = QGst::ElementFactory::make("filesrc");
	m_dec = QGst::ElementFactory::make("decodebin2");
	queuea = QGst::ElementFactory::make("queue", "Queue audio");
	bus = m_pipeline->bus();
	bus->addSignalWatch();
	QGlib::connect(bus, "message", this, &QGstAudioPlayer::onBusMessage);
	QGlib::connect(m_dec, "new-decoded-pad", this, &QGstAudioPlayer::new_decoded_pad);
	QGlib::connect(m_dec, "autoplug_continue", this, &QGstAudioPlayer::autoplug_continue);
	m_pipeline->add(m_src, m_dec);
	m_src->link(m_dec);
	
	m_audiobin = QGst::Bin::create("-Audio bin");
	conv = QGst::ElementFactory::make("audioconvert");
        resample = QGst::ElementFactory::make("audioresample");
        m_volume = QGst::ElementFactory::make("volume");
	asink = QGst::ElementFactory::make("autoaudiosink", "Auto Audio Sink");
        m_audiobin->add(queuea, conv, resample, m_volume, asink);
	QGst::PadPtr audiopad = queuea->getStaticPad("sink"); 
	queuea->link(conv);
        conv->link(resample);
        resample->link(m_volume);
        m_volume->link(asink);
	QGst::GhostPadPtr gpad = QGst::GhostPad::create(audiopad, "sink");
	m_audiobin->addPad(gpad);
	
        qDebug() << "End audio definition, start video";
	m_videobin = QGst::Bin::create("-Video bin");
	m_vsink = QGst::ElementFactory::make("fakesink", "Auto video Sink");
	m_vsink->setProperty<bool>("sync", true);
	queuev = QGst::ElementFactory::make("queue", "Queue video");
	m_videobin->add(queuev, m_vsink);
	QGst::GhostPadPtr gpad2 = QGst::GhostPad::create(queuev->getStaticPad("sink"), "sink");
	m_videobin->addPad(gpad2);
	queuev->link(m_vsink);

	m_pipeline->add(m_audiobin, m_videobin);
	
	return true;
}

void	QGstAudioPlayer::open(const QString file)
{
    QFileInfo   fi(file);
    if (fi.exists())
        m_src->setProperty("location", file);
    else {
        emit error(SQError(SQError::Critical, "Can't locate : " + file));
        return;
    }
}

void	QGstAudioPlayer::seek(const int toseek)
{
}

void	QGstAudioPlayer::play()
{
    qDebug() << "Hi I am GGstAudioPlayer and I am in thread : " << QThread::currentThreadId();
    if (m_pipeline->setState(QGst::StatePlaying) == QGst::StateChangeFailure) {
        emit error(SQError(SQError::Critical, "Failed to change state to play"));
        return ;
     }
    m_framenb = -1;
    qDebug() << "end of gstaudioplay";
}

void	QGstAudioPlayer::stop()
{
    m_timer->stop();
    if (m_pipeline->setState(QGst::StateNull) == QGst::StateChangeFailure) {
        emit error(SQError(SQError::Critical, "Failed to change state to NULL"));
        return ;
    }
    m_framenb = -1;
}

void	QGstAudioPlayer::checkFrame()
{
    QGst::PositionQueryPtr query = QGst::PositionQuery::create(QGst::FormatTime);
    m_pipeline->query(query);
    int tmp = ((query->position() / 1000000) * framerate) / 1000;
        if (m_framenb != tmp)
	{
		m_framenb = tmp;
		emit frameChanged(m_framenb);
	}
}

QDebug  operator<<(QDebug dbg, const QGst::State &st)
{
	if (st == QGst::StatePaused)
		dbg << "Paused";
	if (st == QGst::StatePlaying)
		dbg << "Playing";
	if (st == QGst::StateReady)
		dbg << "Ready";
	if (st == QGst::StateNull)
		dbg << "Null";
	if (st == QGst::StateVoidPending)
		dbg << "VoidPending";
	return dbg;
}


FilePlayer * QGstAudioPlayer::getMe()
{
    return this;
}

void QGstAudioPlayer::dispose()
{
    QGst::cleanup();
}

QGstAudioPlayer::~QGstAudioPlayer()
{
    dispose();
}

bool QGstAudioPlayer::hasVolumeControl()
{
    return true;
}

void QGstAudioPlayer::setVolume(uint vol)
{
    m_volume->setProperty("volume", (float)((float)vol / 100.0));
}


Q_EXPORT_PLUGIN2(qtoyunda_qgstaudioplayer, QGstAudioPlayer)


