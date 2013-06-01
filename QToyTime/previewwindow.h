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

#ifndef PREVIEWWINDOW_H
#define PREVIEWWINDOW_H

#include <QWidget>
#include "videowidget.h"

class PreviewWindow : public QWidget
{
    Q_OBJECT
public:
    explicit PreviewWindow(QWidget *parent = 0);
    void    setVideoFile(QString filePath);
    void    setSubFile(QString filePath);
    void    setSubText(QString subText);
    void    setPosition(const QTime& pos);
    bool    init(QString vSink = QString());
    
signals:
    
public slots:
    void    play();
    void    pause();


private:
    VideoWidget*    m_videoWidget;

    QTime           m_prevPos;

    void            keyPressEvent(QKeyEvent *);
    void            closeEvent(QCloseEvent *);
    
};

#endif // PREVIEWWINDOW_H
