#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H

#include <QWidget>
#include <QTime>
#include <QGst/Ui/VideoWidget>
#include <QGst/Pipeline>
#include <QGst/Element>
#include <QGst/Bin>
#include <QKeyEvent>
#include <QTimer>



class VideoPlayer : public QGst::Ui::VideoWidget
{
    Q_OBJECT
public:
    explicit VideoPlayer(QString videoSink, QWidget *parent = 0);
    ~VideoPlayer();

    bool    init(QString videoSink = QString());
    void    setPosition(const QTime &pos);
    QTime   position() const;
    QTime   duration() const;
    //QGst::State state() const;*/
    void    setVideoFile(QString file);
    void    setSubFile(QString file);
    void    resizeVideo(int, int);
    //static  bool    GstInit(int& ac, char **ag[]);
    void    play();

    void calcScaledSize(int wanted_width, int wanted_height, int *m_scaled_widht, int *m_scaled_height);

private:
    void    onBusMessage(const QGst::MessagePtr & message);

    void    new_decoded_pad(const QGst::PadPtr &, const int);
    bool    autoplug_continue(const QGst::PadPtr &, const QGst::CapsPtr &);
    void    keyPressEvent(QKeyEvent *ev);
    void    closeEvent(QCloseEvent *);
    void    resizeEvent(QResizeEvent *);


    QGst::PipelinePtr   m_pipeline;
    QGst::ElementPtr    m_dec;
    QGst::ElementPtr    m_src;
    QGst::ElementPtr    m_capsFilter;
    QGst::BinPtr        m_videobin;
    QGst::BinPtr        m_audiobin;
    QGst::ElementPtr    m_vsink;
    QGst::ElementPtr    m_toyunda;
    double              m_framerate;
    QString             m_subFile;
    QGst::State         m_currentState;
    QTimer*             m_posTimer;
    bool                m_videosink_set;
    int                 original_width;
    int                 original_height;
};

#endif // VIDEOPLAYER_H
