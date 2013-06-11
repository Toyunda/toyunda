#include "videoplayer.h"
#include <QGst/Init>
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    _putenv(QString("GST_PLUGIN_PATH=" + QString(qApp->applicationDirPath() + "/gsttoyunda/")).toLatin1().constData());
    try {
        QGst::init(&argc, &argv);
    } catch (QGlib::Error qgerr)
    {
        qCritical() << "Can't init gstreamer";
        return 1;
    }
    VideoPlayer w("directdrawsink");
    w.setVideoFile("X:/Time/My Little Pony Friendship is Magic - Becoming Popular.avi");
    w.setSubFile("X:/Bokusatsu tenshi Dokuro-chan - OP.txt");
    w.play();
    w.show();
    return a.exec();
}
