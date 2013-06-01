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

#include "qgplayer.h"
#include "ui_qgplayer.h"

QGPlayer::QGPlayer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::qgplayer)
{
    ui->setupUi(this);
    m_videoWidget = ui->video;
    connect(m_videoWidget, SIGNAL(paused()), this, SLOT(videoPaused()));
    connect(m_videoWidget, SIGNAL(playing()), this, SLOT(videoPlaying()));
    connect(m_videoWidget, SIGNAL(stopped()), this, SLOT(videoStopped()));
    connect(m_videoWidget, SIGNAL(ready()), this, SLOT(videoReady()));
    connect(m_videoWidget, SIGNAL(positionChanged()), this, SLOT(videoPositionChanged()));
    connect(ui->positionSlider, SIGNAL(sliderMoved(int)), this, SLOT(sliderMoved(int)));
    setFocusPolicy(Qt::StrongFocus);
}

QGPlayer::~QGPlayer()
{
    delete ui;
}

VideoWidget *QGPlayer::videoWidget()
{
    return ui->video;
}

void QGPlayer::setVideo(QString video)
{
    ui->video->setVideoFile(video);
    ui->video->pause();
    ui->titleLabel->setText(video);
}


void QGPlayer::keyPressEvent(QKeyEvent *ev)
{
    if (ev->key() == Qt::Key_Space)
    {
        if (m_videoWidget->state() == QGst::StatePlaying)
            m_videoWidget->pause();
        else
        {
            m_videoWidget->play();
        }
    }
    if (ev->key() == Qt::Key_Right)
        m_videoWidget->setPosition(m_videoWidget->position().addSecs(20));
    if (ev->key() == Qt::Key_Left)
        m_videoWidget->setPosition(m_videoWidget->position().addSecs(-20));
}


/*
if (m_rawVideo->state() != QGst::StateNull && m_rawVideo->state() != QGst::StateReady)
{
    QTime tpos;
    qDebug() << pos;
    tpos = tpos.addMSecs((double) pos / 1000 * m_rawVideo->duration().msecsTo(QTime()) * -1);
    qDebug() << tpos;
    m_rawVideo->setPosition(tpos);
}*/

void QGPlayer::on_playPauseButton_clicked()
{
    if (m_videoWidget->state() == QGst::StatePlaying)
        m_videoWidget->pause();
    if (m_videoWidget->state() == QGst::StatePaused || m_videoWidget->state() == QGst::StateNull)
        m_videoWidget->play();
}

void QGPlayer::on_stopButton_clicked()
{
    m_videoWidget->pause();
    m_videoWidget->setPosition(QTime());
}

void QGPlayer::on_seekBackwardButton_clicked()
{
    m_videoWidget->setPosition(m_videoWidget->position().addSecs(-20));
}

void QGPlayer::on_seekForwardButton_clicked()
{
    m_videoWidget->setPosition(m_videoWidget->position().addSecs(20));
}


void QGPlayer::videoPaused()
{
    ui->playPauseButton->setIcon(QIcon(":/main/icons/player_play.svgz"));
    emit    paused();
}

void QGPlayer::videoPlaying()
{
    ui->playPauseButton->setIcon(QIcon(":/main/icons/player_pause.svgz"));
    emit    playing();
}

void QGPlayer::videoStopped()
{
    ui->playPauseButton->setIcon(QIcon(":/main/icons/player_play.svgz"));
}

void QGPlayer::videoReady()
{
    ui->positionLabel->setText(tr("Position : ") + m_videoWidget->position().toString() + "/" + m_videoWidget->duration().toString());
}

void QGPlayer::videoPositionChanged()
{
    QTime length(0,0);
    QTime curpos(0,0);

    if (m_videoWidget->state() != QGst::StateReady &&
            m_videoWidget->state() != QGst::StateNull)
    {
        length = m_videoWidget->duration();
        curpos = m_videoWidget->position();
    }
    if (length != QTime(0,0)) {
        ui->positionSlider->setValue(curpos.msecsTo(QTime()) * 1000 / length.msecsTo(QTime()));
        ui->positionLabel->setText(tr("Position : ") + curpos.toString());
    } else {
        ui->positionSlider->setValue(0);
    }
    ui->positionLabel->setText(QString(tr("Position : %1/%2 -- Frame : %3")).arg(curpos.toString()).arg(length.toString()).arg(m_videoWidget->positionFrame()));
}

void QGPlayer::sliderMoved(int pos)
{
    QTime tpos;
    tpos = tpos.addMSecs((double) pos / 1000 * m_videoWidget->duration().msecsTo(QTime()) * -1);
    m_videoWidget->setPosition(tpos);
}
