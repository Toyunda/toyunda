#include "videoplayer.h"
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
#include <QKeyEvent>
#include <QDesktopWidget>


void	VideoPlayer::calcScaledSize(int wanted_width, int wanted_height, int* m_scaled_widht, int* m_scaled_height)
{
    float	aspect_ratio = (float) original_width / original_height;
    float 	wanted_ratio = (float) wanted_width / wanted_height;

    qDebug() << QString("  calc_scaled_size : Original : %1x%2 , Wanted : %3x%4").arg(original_width)
                            .arg(original_height).arg(wanted_width).arg(wanted_height);
    if (wanted_ratio < aspect_ratio)
    {
        *m_scaled_widht = wanted_width;
        *m_scaled_height = ((float)wanted_width / original_width) * original_height;
    }
    else
    {
        *m_scaled_height = wanted_height;
        *m_scaled_widht = ((float)wanted_height / original_height) * original_width;
    }
}

VideoPlayer::VideoPlayer(QString videoSink, QWidget *parent)
    : QGst::Ui::VideoWidget(parent)
{
    QGst::BusPtr bus;
    QGst::ElementPtr conv, asink, queuea, queuev, resample, vscale, recolor;

    m_pipeline = QGst::Pipeline::create();
    m_src = QGst::ElementFactory::make("filesrc");
    m_dec = QGst::ElementFactory::make("decodebin");
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
    m_toyunda = QGst::ElementFactory::make("toyunda", "Toyunda");
    m_capsFilter = QGst::ElementFactory::make("capsfilter", "CapsFilter");
    vscale = QGst::ElementFactory::make("videoscale", "videoscale");
    recolor = QGst::ElementFactory::make("videoconvert", "ffmegcolorspace");
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
    CHECK_GST_IS_NULL(vscale);
    CHECK_GST_IS_NULL(recolor);
    CHECK_GST_IS_NULL(queuev);
    CHECK_GST_IS_NULL(m_capsFilter);

    bus = m_pipeline->bus();
    bus->addSignalWatch();
    m_dec->setProperty<int>("connection-speed", 56);
    QGlib::connect(bus, "message", this, &VideoPlayer::onBusMessage);
    QGlib::connect(m_dec, "added-pad", this, &VideoPlayer::pad_added);
    QGlib::connect(m_dec, "autoplug_continue", this, &VideoPlayer::autoplug_continue);
    m_pipeline->add(m_src, m_dec);
    m_src->link(m_dec);

    m_audiobin = QGst::Bin::create("-Audio bin");
    m_audiobin->add(queuea, conv, resample, asink);
    QGst::PadPtr audiopad = queuea->getStaticPad("sink");
    queuea->link(conv);
    conv->link(resample);
    resample->link(asink);
    QGst::GhostPadPtr gpad = QGst::GhostPad::create(audiopad, "sink");
    m_audiobin->addPad(gpad);
    m_videobin = QGst::Bin::create("-Video bin");

    m_vsink->setProperty<bool>("sync", true);
    m_videobin->add(queuev, vscale, m_toyunda, recolor, m_vsink, m_capsFilter);
    QGst::GhostPadPtr gpad2 = QGst::GhostPad::create(queuev->getStaticPad("sink"), "sink");
    m_videobin->addPad(gpad2);
    queuev->link(vscale);
    vscale->link(m_capsFilter);
    m_capsFilter->link(m_toyunda);
    m_toyunda->link(recolor);
    recolor->link(m_vsink);

    m_pipeline->add(m_audiobin, m_videobin);
    m_currentState = QGst::StateNull;


    m_toyunda->setProperty("toyunda-logo", qApp->applicationDirPath().toUtf8() + "/toyunda.tga");
    m_videosink_set = false;
    m_vsink->setProperty("force-aspect-ratio", true);
    //setVideoSink(m_vsink);
    watchPipeline(m_pipeline);
    fullscreenRequest = false;
    setFixedSize(800, 600);
}

void	VideoPlayer::pad_added(const QGst::PadPtr &pad)
{
    QGst::PadPtr	audiopad;
    QGst::PadPtr	videopad;
    QGst::CapsPtr	caps;
    QGst::StructurePtr str;

    audiopad = m_audiobin->getStaticPad("sink");
    videopad = m_videobin->getStaticPad("sink");
    caps = pad->currentCaps();
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

bool	VideoPlayer::autoplug_continue(const QGst::PadPtr &pad, const QGst::CapsPtr &cap)
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
    if (str->hasField("height"))
    {
        original_height = str->value("height").get<int>();
        original_width = str->value("width").get<int>();
        qDebug() << QString("video size : %1x%2").arg(original_width).arg(original_height);
        resizeVideo(800, 600);
    }
    return true;
}

void VideoPlayer::keyPressEvent(QKeyEvent *ev)
{
    if (ev->key() == Qt::Key_F)
    {
        setWindowState(windowState() ^ Qt::WindowFullScreen);
        qDebug() << "FULLSCREEN";
        /*if (!(windowState() & Qt::WindowFullScreen))
           resizeVideo(800, 600);
        else
           resizeVideo(qApp->desktop()->screenGeometry().width(), qApp->desktop()->screenGeometry().height());*/
    }
    if (ev->key() == Qt::Key_Q)
	    close();
    if (ev->key() == Qt::Key_Escape)
	    close();
    if (ev->key() == Qt::Key_Left)
	    setPosition(position().addSecs(-10));
    if (ev->key() == Qt::Key_Right)
	    setPosition(position().addSecs(10));

}

void VideoPlayer::closeEvent(QCloseEvent *)
{
    if (m_pipeline)
        m_pipeline->setState(QGst::StateNull);
}

void VideoPlayer::resizeEvent(QResizeEvent *ev)
{

    /*qDebug() << "RESIZE";
    if (fullscreenRequest)
    {
       resizeVideo(ev->size().width(), ev->size().height());
       fullscreenRequest = false;
    }*/
}

void VideoPlayer::onBusMessage(const QGst::MessagePtr &message)
{
    switch (message->type()) {
    case QGst::MessageEos: //End of stream. We reached the end of the file.
        //stop();
        break;
    case QGst::MessageError: //Some error occurred.
        qCritical() << message.staticCast<QGst::ErrorMessage>()->error();
        //stop();
        break;
    case QGst::MessageStateChanged: //The element in message->source() has changed state
        if (message->source() == m_pipeline) {
            QGst::StateChangedMessagePtr sm = message.staticCast<QGst::StateChangedMessage>();
            m_currentState = sm->newState();
        }
        break;
    default:
        break;
    }
}

void VideoPlayer::setVideoFile(QString file)
{
    if (m_currentState != QGst::StateNull)
       m_pipeline->setState(QGst::StateReady);
    m_src->setProperty("location", file);
    m_pipeline->setState(QGst::StateReady);
}

void VideoPlayer::setSubFile(QString file)
{
    m_subFile = file;
    m_toyunda->setProperty("subfile", file);
    qDebug() << file;
}

void VideoPlayer::setToyundaImagePath(QString imgPath)
{
    qDebug() << "IMGPATH" << imgPath;
    m_toyunda->setProperty("image-base-dir", imgPath);
}

void VideoPlayer::resizeVideo(int ww, int wh)
{
    QGst::CapsPtr cp = QGst::Caps::createSimple("video/x-raw-yuv");
    int w, h;
    calcScaledSize(ww, wh, &w, &h);
    cp->setValue("width", w);
    cp->setValue("height", h);
    qDebug() << w << h;
    //QGst::State = m_pipeline->getState();
    m_capsFilter->setProperty("caps", cp);
}

void VideoPlayer::play()
{
    if (m_pipeline)
        m_pipeline->setState(QGst::StatePlaying);
}

VideoPlayer::~VideoPlayer()
{

}

void VideoPlayer::setPosition(const QTime &pos)
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

QTime VideoPlayer::position() const
{
	if (m_pipeline) {
		//here we query the pipeline about its position
		//and we request that the result is returned in time format
		QGst::PositionQueryPtr query = QGst::PositionQuery::create(QGst::FormatTime);
		m_pipeline->query(query);
		QGst::ClockTime ct(query->position());
		if (ct.isValid())
			return ct.toTime();
		else
			return QTime(25, 70);
	} else {
		return QTime();
	}
}

QTime VideoPlayer::duration() const
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
