#include "videoplayer.h"
#include "../comons/sqhandlegstpath.h"
#include <QGst/Init>
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    sq_add_gsttoyunda_plugin_path(qApp->applicationDirPath());
    //try {
        QGst::init(&argc, &argv);
    //} catch (QGlib::Error qgerr)
    //{
    //    qCritical() << "Can't init gstreamer";
    //    return 1;
    //}
    VideoPlayer w("xvimagesink");
    w.setVideoFile(argv[1]);
    w.setSubFile(argv[2]);
    w.play();
    w.show();
    return a.exec();
}
