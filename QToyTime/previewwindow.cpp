#include "previewwindow.h"

PreviewWindow::PreviewWindow(QWidget *parent) :
    QWidget(parent)
{
    m_videoWidget = new VideoWidget(this);
    m_videoWidget->setFixedSize(800, 600);
    setFixedSize(800, 600);
    setWindowFlags(Qt::Window);
    setWindowTitle("QToyTime Preview Window");
}

void PreviewWindow::setVideoFile(QString filePath)
{
    m_videoWidget->setVideoFile(filePath);
}

void PreviewWindow::setSubFile(QString filePath)
{
    m_videoWidget->setSubFile(filePath);
}

void PreviewWindow::setSubText(QString subText)
{
    ;
}

void PreviewWindow::setPosition(const QTime &pos)
{
    m_prevPos = pos;
    m_videoWidget->setPosition(pos);
}

bool PreviewWindow::init(QString vSink)
{
    m_videoWidget->init(vSink);
    m_videoWidget->activateSubtitle(true);
}


void PreviewWindow::play()
{
    m_videoWidget->play();
}

void PreviewWindow::pause()
{
    m_videoWidget->pause();
}

void PreviewWindow::keyPressEvent(QKeyEvent *ev)
{
    if (ev->key() == Qt::Key_Backspace)
        m_videoWidget->setPosition(m_prevPos);
    if (ev->key() == Qt::Key_Space)
    {
        if (m_videoWidget->state() == QGst::StatePlaying)
            pause();
        else
            play();
    }
    if (ev->key() == Qt::Key_Escape || ev->key() == Qt::Key_Q)
    {
        pause();
        hide();
    }
    if (ev->key() == Qt::Key_Right)
        m_videoWidget->setPosition(m_videoWidget->position().addSecs(20));
    if (ev->key() == Qt::Key_Left)
        m_videoWidget->setPosition(m_videoWidget->position().addSecs(-20));
}

void PreviewWindow::closeEvent(QCloseEvent *)
{
    m_videoWidget->stop();
}
