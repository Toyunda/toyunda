#include "qtoytime.h"
#include <QApplication>
#include <QTranslator>
#include "videowidget.h"
#include "../comons/sqhandlegstpath.h"
#include "generatetoyundasubtitle.h"

static QTextStream logfile;
static QTextStream cout(stdout);

void myMessageOutput(QtMsgType type, const char *msg)
{
    switch (type)
    {
        case QtDebugMsg:
            logfile << "Debug : " << msg;
            break;
        case QtCriticalMsg:
            logfile << "Critical : " << msg;
            break;
        case QtWarningMsg:
            logfile << "Warning : " << msg;
            break;
        case QtFatalMsg:
            logfile << "Fatal : " << msg;
            break;
    }
    logfile << "\n";
    logfile.flush();
    cout << msg << "\n";
    cout.flush();
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFile   mlog(qApp->applicationDirPath().toLocal8Bit() + "/qtoytimelog.txt");
    logfile.setDevice(&mlog);
    if (mlog.open(QIODevice::WriteOnly | QIODevice::Text))
        qInstallMsgHandler(myMessageOutput);
    sq_set_gstlib_env();
    sq_add_gsttoyunda_plugin_path(qApp->applicationDirPath());
    qDebug() << getenv("GST_PLUGIN_PATH");

    QString locale = QLocale::system().name().split('_').first();
    qDebug() << locale;

    QTranslator translator;
    translator.load(qApp->applicationDirPath() + "/i18n/" + locale + "_QToyTime.qm" );
    a.installTranslator(&translator);
    VideoWidget::GstInit(argc, &argv);
    QToyTime w;

    //generateToyundaSubtitle("&il &fait &beau\n&le &temps &sec\n&c'est &cool", "100 120\n200 220\n240 245\n250 255\n260 265\n270 280\n285 290\n291 297\n", "tmp.txt");
    //return 0;
    w.show();
    
    return a.exec();
}
