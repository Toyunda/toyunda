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
    void    sliderMoved(int pos);


    void on_playPauseButton_clicked();

    void on_stopButton_clicked();

    void on_seekBackwardButton_clicked();

    void on_seekForwardButton_clicked();


private:
    Ui::qgplayer *ui;
    VideoWidget*    m_videoWidget;

    void    keyPressEvent(QKeyEvent *ev);

};

#endif // QGPLAYER_H
