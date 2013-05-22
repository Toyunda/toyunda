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
