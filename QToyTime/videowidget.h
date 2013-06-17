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

#ifndef VIDEOWIDGET_H
#define VIDEOWIDGET_H

#include <QWidget>
#include <QTime>
#include <QGst/Ui/VideoWidget>
#include <QGst/Pipeline>
#include <QGst/Element>
#include <QGst/Bin>
#include <QKeyEvent>
#include <QTimer>



class VideoWidget : public QGst::Ui::VideoWidget
{
    Q_OBJECT
public:
    explicit VideoWidget(QWidget *parent = 0);
    ~VideoWidget();

    bool    init(QString videoSink = QString());
    void    setPosition(const QTime &pos);
    void    setPosition(int frame);
    QTime   getTimeFromFrame(int) const;
    int     positionFrame() const;
    QTime   position() const;
    QTime   duration() const;
    QGst::State state() const;
    void    setVideoFile(QString file);
    void    setSubFile(QString file);
    void    reloadSub();
    void    activateSubtitle(bool act);
    double  framerate() const;
    static  bool    GstInit(int& ac, char **ag[]);
    int     volume() const;
    void    setVolume(int);
    void    setSpeed(int);

    
signals:
    void    paused();
    void    playing();
    void    stopped();
    void    ready();
    void    videoMouseClickPress(int);
    void    videoMouseClickRelease(int);
    void    positionChanged();
    void    keyEvent(QKeyEvent &);

public slots:
    void    pause();
    void    play();
    void    stop();

private:
    void    onBusMessage(const QGst::MessagePtr & message);
    void    handlePipelineStateChange(const QGst::StateChangedMessagePtr & scm);

    void    new_decoded_pad(const QGst::PadPtr &, const int);
    bool    autoplug_continue(const QGst::PadPtr &, const QGst::CapsPtr &);
    void    mousePressEvent(QMouseEvent *);
    void    mouseReleaseEvent(QMouseEvent *);
    void    keyPressEvent(QKeyEvent *ev);

    QGst::PipelinePtr   m_pipeline;
    QGst::ElementPtr    m_dec;
    QGst::ElementPtr    m_src;
    QGst::BinPtr        m_videobin;
    QGst::BinPtr        m_audiobin;
    QGst::ElementPtr    m_vsink;
    QGst::ElementPtr    m_toyunda;
    QGst::ElementPtr    m_volume;
    double              m_framerate;
    QString             m_subFile;
    QGst::State         m_currentState;
    QTimer*             m_posTimer;
    bool                m_videosink_set;
};

#endif // VIDEOWIDGET_H
