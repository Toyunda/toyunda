/*
    This file is part of QToyunda software
    Copyright (C) 2013  Sylvain "Skarsnik" Colinet <scolinet@gmail.com>

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
#include <QGst/Init>
#include <QGst/ElementFactory>
#include <QGst/Bin>
#include <QGst/Pad>
#include <QGst/Bus>
#include <QGst/Query>
#include <QGst/GhostPad>
#include <QGlib/Connect>
#include <QGst/ClockTime>
#include <QGlib/Connect>
#include <QGlib/Error>
#include <QGst/Fraction>
#include <QGst/Event>
#include <QGst/Message>

#include <QApplication>
#include "videowidget.h"
#include <QKeyEvent>

VideoWidget::VideoWidget(QWidget *parent) :
    QGst::Ui::VideoWidget(parent)
{
    //setFocusPolicy(Qt::ClickFocus);
    m_posTimer = new QTimer();
    connect(m_posTimer, SIGNAL(timeout()), this, SIGNAL(positionChanged()));
}

bool VideoWidget::init(QString videoSink)
{
    QGst::BusPtr bus;
    QGst::ElementPtr conv, asink, queuea, queuev, resample, vscale, recolor;

    m_pipeline = QGst::Pipeline::create();
    m_src = QGst::ElementFactory::make("filesrc");
    m_dec = QGst::ElementFactory::make("decodebin2");
    queuea = QGst::ElementFactory::make("queue", "Queue audio");

    conv = QGst::ElementFactory::make("audioconvert");
    resample = QGst::ElementFactory::make("audioresample");
#ifdef  Q_WS_WIN
    asink = QGst::ElementFactory::make("directsoundsink", "Auto Audio Sink");
    if (videoSink.isEmpty())
        m_vsink = QGst::ElementFactory::make("dshowvideosink", "video sink");
    else
        m_vsink = QGst::ElementFactory::make(videoSink, "video sink");
#else
    asink = QGst::ElementFactory::make("autoaudiosink", "Auto Audio Sink");
    if (videoSink.isEmpty())
        m_vsink = QGst::ElementFactory::make("autovideosink", "video sink");
    else
        m_vsink = QGst::ElementFactory::make(videoSink, "video sink");
#endif
    m_volume = QGst::ElementFactory::make("volume", "volume");
    m_toyunda = QGst::ElementFactory::make("toyunda", "Toyunda");
    vscale = QGst::ElementFactory::make("videoscale", "videoscale");
    recolor = QGst::ElementFactory::make("ffmpegcolorspace", "ffmegcolorspace");
    queuev = QGst::ElementFactory::make("queue", "Queue video");

#define CHECK_GST_IS_NULL(gstElem) if (gstElem.isNull()) { qCritical() << "Error creating " << #gstElem;}

    CHECK_GST_IS_NULL(m_src);
    CHECK_GST_IS_NULL(m_dec);
    CHECK_GST_IS_NULL(conv);
    CHECK_GST_IS_NULL(queuea);
    CHECK_GST_IS_NULL(resample);
    CHECK_GST_IS_NULL(asink);
    CHECK_GST_IS_NULL(m_vsink);
    CHECK_GST_IS_NULL(m_toyunda);
    CHECK_GST_IS_NULL(m_volume);
    CHECK_GST_IS_NULL(vscale);
    CHECK_GST_IS_NULL(recolor);
    CHECK_GST_IS_NULL(queuev);

    bus = m_pipeline->bus();
    bus->addSignalWatch();
    m_dec->setProperty<int>("connection-speed", 56);
    QGlib::connect(bus, "message", this, &VideoWidget::onBusMessage);
    QGlib::connect(m_dec, "new-decoded-pad", this, &VideoWidget::new_decoded_pad);
    QGlib::connect(m_dec, "autoplug_continue", this, &VideoWidget::autoplug_continue);
    m_pipeline->add(m_src, m_dec);
    m_src->link(m_dec);

    m_audiobin = QGst::Bin::create("-Audio bin");
    m_audiobin->add(queuea, conv, resample, asink, m_volume);
    QGst::PadPtr audiopad = queuea->getStaticPad("sink");
    queuea->link(conv);
    conv->link(resample);
    resample->link(m_volume);
    m_volume->link(asink);
    QGst::GhostPadPtr gpad = QGst::GhostPad::create(audiopad, "sink");
    m_audiobin->addPad(gpad);
    m_videobin = QGst::Bin::create("-Video bin");

    m_vsink->setProperty<bool>("sync", true);
    m_videobin->add(queuev, vscale, m_toyunda, recolor, m_vsink);
    QGst::GhostPadPtr gpad2 = QGst::GhostPad::create(queuev->getStaticPad("sink"), "sink");
    m_videobin->addPad(gpad2);
    queuev->link(m_toyunda);
    m_toyunda->link(recolor);
    recolor->link(vscale);
    vscale->link(m_vsink);

    m_pipeline->add(m_audiobin, m_videobin);
    m_currentState = QGst::StateNull;


    m_toyunda->setProperty("toyunda-logo", qApp->applicationDirPath().toUtf8() + "/toyunda.tga");
    m_videosink_set = false;
    m_vsink->setProperty("force-aspect-ratio", true);
    watchPipeline(m_pipeline);
    return true;
}


VideoWidget::~VideoWidget()
{
    if (m_pipeline)
        m_pipeline->setState(QGst::StateNull);
}


void VideoWidget::setPosition(const QTime &pos)
{
    QTime npos;
    if (pos <= duration())
        npos = pos;
    QGst::SeekEventPtr evt = QGst::SeekEvent::create(
        1.0, QGst::FormatTime, QGst::SeekFlagFlush,
        QGst::SeekTypeSet, QGst::ClockTime::fromTime(npos),
        QGst::SeekTypeNone, QGst::ClockTime::None
    );

    m_pipeline->sendEvent(evt);
}

void VideoWidget::setPosition(int frame)
{
    QTime pos;
    pos = pos.addMSecs(((double) frame / m_framerate) * 1000);
    QGst::SeekEventPtr evt = QGst::SeekEvent::create(
        1.0, QGst::FormatTime, QGst::SeekFlagFlush,
        QGst::SeekTypeSet, QGst::ClockTime::fromTime(pos),
        QGst::SeekTypeNone, QGst::ClockTime::None
    );
    m_pipeline->sendEvent(evt);
}

QTime VideoWidget::getTimeFromFrame(int frameNb) const
{
    QTime pos;
    double  tmp = ((double) frameNb / m_framerate) * 1000;
    pos = pos.addMSecs((int) tmp);
    return pos;
}

int VideoWidget::positionFrame() const
{
    if (m_pipeline) {
        //here we query the pipeline about its position
        //and we request that the result is returned in time format
        QGst::PositionQueryPtr query = QGst::PositionQuery::create(QGst::FormatTime);
        m_pipeline->query(query);
        return ((query->position() / 1000000) * m_framerate + 100) / 1000;
    } else {
        return -1;
    }
}

QTime VideoWidget::position() const
{
    if (m_pipeline) {
        //here we query the pipeline about its position
        //and we request that the result is returned in time format
        QGst::PositionQueryPtr query = QGst::PositionQuery::create(QGst::FormatTime);
        m_pipeline->query(query);
        return QGst::ClockTime(query->position()).toTime();
    } else {
        return QTime();
    }
}

void	VideoWidget::new_decoded_pad(const QGst::PadPtr &pad, const int gbool)
{
    Q_UNUSED(gbool);
    QGst::PadPtr	audiopad;
    QGst::PadPtr	videopad;
    QGst::CapsPtr	caps;
    QGst::StructurePtr str;

    audiopad = m_audiobin->getStaticPad("sink");
    videopad = m_videobin->getStaticPad("sink");
    caps = pad->caps();
    str = caps->internalStructure(0);
    qDebug() << str->name();
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

bool	VideoWidget::autoplug_continue(const QGst::PadPtr &pad, const QGst::CapsPtr &cap)
{
    QGst::StructurePtr str = cap->internalStructure(0);
        //qDebug() << "Name : " << pad->name() << "Cap :" << cap;
    if (str->hasField("framerate"))
    {
       QGst::Fraction frac = str->value("framerate").get<QGst::Fraction>();
       qDebug() << "Framerate : " << frac.numerator << "/" << frac.denominator;
       m_framerate = ((double) frac.numerator / frac.denominator);
       qDebug() << m_framerate;
    }
    return true;
}

void VideoWidget::mousePressEvent(QMouseEvent *ev)
{
    Q_UNUSED(ev);
    if (m_pipeline)
    {
        QGst::PositionQueryPtr query = QGst::PositionQuery::create(QGst::FormatTime);
        m_pipeline->query(query);
        int tmp = ((query->position() / 1000000) * m_framerate + 100) / 1000;
        emit videoMouseClickPress(tmp);
    }
}

void VideoWidget::mouseReleaseEvent(QMouseEvent *ev)
{
    Q_UNUSED(ev);
    if (m_pipeline)
    {
        QGst::PositionQueryPtr query = QGst::PositionQuery::create(QGst::FormatTime);
        m_pipeline->query(query);
        int tmp = ((query->position() / 1000000) * m_framerate + 100) / 1000;
        emit videoMouseClickRelease(tmp);
    }
}

void VideoWidget::keyPressEvent(QKeyEvent *ev)
{
    emit    keyEvent(*ev);
}


void VideoWidget::play()
{
    if (m_pipeline) {
        m_pipeline->setState(QGst::StatePlaying);
    }
}

void    VideoWidget::pause()
{
    if (m_pipeline) {
        m_pipeline->setState(QGst::StatePaused);
    }
}

void    VideoWidget::stop()
{
    if (m_pipeline) {
        m_pipeline->setState(QGst::StateNull);
        emit stopped();
    }
}

void VideoWidget::onBusMessage(const QGst::MessagePtr &message)
{
    switch (message->type()) {
    case QGst::MessageEos: //End of stream. We reached the end of the file.
        stop();
        break;
    case QGst::MessageError: //Some error occurred.
        qCritical() << message.staticCast<QGst::ErrorMessage>()->error();
        stop();
        break;
    case QGst::MessageStateChanged: //The element in message->source() has changed state
        if (message->source() == m_pipeline) {
            handlePipelineStateChange(message.staticCast<QGst::StateChangedMessage>());
        }
        break;
    default:
        break;
    }
}


double VideoWidget::framerate() const
{
    return m_framerate;
}

bool VideoWidget::GstInit(int &ac, char **ag[])
{
    try {
        QGst::init(&ac, ag);
    } catch (QGlib::Error qgerr)
    {
        qCritical() << "Can't init gstreamer";
        return false;
    }
    return true;
}


QTime VideoWidget::duration() const
{
    if (m_pipeline) {
        //here we query the pipeline about the content's duration
        //and we request that the result is returned in time format
        QGst::DurationQueryPtr query = QGst::DurationQuery::create(QGst::FormatTime);
        m_pipeline->query(query);
        return QGst::ClockTime(query->duration()).toTime();
    } else {
        return QTime();
    }
}

QGst::State VideoWidget::state() const
{
    return m_currentState;
}

void VideoWidget::setVideoFile(QString file)
{
    if (m_currentState != QGst::StateNull)
       m_pipeline->setState(QGst::StateReady);
    if (!m_videosink_set)
    {
        //setVideoSink(m_vsink);
        //watchPipeline(m_pipeline);
        m_videosink_set = true;
    }
    m_src->setProperty("location", file);
    m_pipeline->setState(QGst::StateReady);
}

void VideoWidget::setSubFile(QString file)
{
    m_subFile = file;
    m_toyunda->setProperty("subfile", file);
}

void VideoWidget::reloadSub()
{
    setSubFile(m_subFile);
}

void VideoWidget::activateSubtitle(bool act)
{
    Q_UNUSED(act);
}


void VideoWidget::handlePipelineStateChange(const QGst::StateChangedMessagePtr &scm)
{
    m_currentState = scm->newState();
    switch (scm->newState()) {
    case QGst::StateReady:
        if (!m_videosink_set)
        {
            //setVideoSink(m_vsink);
            //watchPipeline(m_pipeline);
            m_videosink_set = true;
        }
        qDebug() << "ready";
        emit ready();
        break;
    case QGst::StatePlaying:
        //start the timer when the pipeline starts playing
        emit    playing();
        m_posTimer->start(100);
        break;
    case QGst::StatePaused:
        //stop the timer when the pipeline pauses
        emit    paused();
        if (scm->oldState() == QGst::StatePlaying)
            m_posTimer->stop();
        emit positionChanged();
        break;
    case QGst::StateNull:
        //m_videosink_set = false;
        break;
    default:
        break;
    }
}

int VideoWidget::volume() const
{
    return m_volume->property("volume").get<float>() * 100;
}

void VideoWidget::setVolume(int vol)
{
    m_volume->setProperty("volume", (float)((float)vol / 100.0));
}

void VideoWidget::setSpeed(int sp)
{
    double rate = (float) sp / 100;
    QGst::SeekEventPtr evt = QGst::SeekEvent::create(
        rate, QGst::FormatTime, QGst::SeekFlagFlush,
        QGst::SeekTypeSet, QGst::ClockTime::fromTime(position()),
        QGst::SeekTypeNone, QGst::ClockTime::None
    );
    m_pipeline->sendEvent(evt);
}
