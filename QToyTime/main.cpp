#include "qtoytime.h"
#include <QApplication>
#include "videowidget.h"
#include "../comons/sqhandlegstpath.h"
#include "generatetoyundasubtitle.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    sq_set_gstlib_env();
    sq_add_gsttoyunda_plugin_path(qApp->applicationDirPath());
    qDebug() << getenv("GST_PLUGIN_PATH");
    VideoWidget::GstInit(argc, &argv);
    QToyTime w;

    //generateToyundaSubtitle("&il &fait &beau\n&le &temps &sec\n&c'est &cool", "100 120\n200 220\n240 245\n250 255\n260 265\n270 280\n285 290\n291 297\n", "tmp.txt");
    //return 0;
    w.show();
    
    return a.exec();
}
