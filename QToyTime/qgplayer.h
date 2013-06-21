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

#ifndef QGPLAYER_H
#define QGPLAYER_H

#include <QWidget>
#include "videowidget.h"
#include <QSlider>
#include <QDial>
#include <QFrame>
#include "mslider.h"

namespace Ui {
class qgplayer;
}

class QGPlayer : public QWidget
{
    Q_OBJECT
    
public:
    explicit QGPlayer(QWidget *parent = 0);
    ~QGPlayer();
    VideoWidget*    videoWidget();
    void            setVideo(QString video);


    signals:
    void    paused();
    void    playing();

private slots:
    void    videoPaused();
    void    videoPlaying();
    void    videoStopped();
    void    videoReady();
    void    videoPositionChanged();
    void    videoKeyEvent(QKeyEvent &ev);
    void    sliderMoved(int pos);

    void    on_volumeSliderValueChanged(int);
    void    on_speedSliderValueChanged(int);


    void on_playPauseButton_clicked();

    void on_stopButton_clicked();

    void on_seekBackwardButton_clicked();

    void on_seekForwardButton_clicked();


    void on_volumeButton_clicked();

    void on_speedButton_clicked();

private:
    Ui::qgplayer *ui;
    VideoWidget*    m_videoWidget;
    MSlider*        m_volumeSlider;
    MSlider*        m_speedSlider;
    QFrame*         fm_volume;
    QFrame*         fm_speed;

    void    keyPressEvent(QKeyEvent *ev);

};

#endif // QGPLAYER_H
