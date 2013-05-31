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
