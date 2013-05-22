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
    QTime   position() const;
    QTime   duration() const;
    QGst::State state() const;
    void    setVideoFile(QString file);
    void    setSubFile(QString file);
    void    reloadSub();
    void    activateSubtitle(bool act);
    double  framerate() const;
    static  bool    GstInit(int& ac, char **ag[]);

    
signals:
    void    paused();
    void    playing();
    void    stopped();
    void    videoMouseClickPress(int);
    void    videoMouseClickRelease(int);
    void    keyEvent(QKeyEvent ev);
    void    positionChanged();

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
    double              m_framerate;
    QString             m_subFile;
    QGst::State         m_currentState;
    QTimer*             m_posTimer;
    bool                m_videosink_set;
};

#endif // VIDEOWIDGET_H
