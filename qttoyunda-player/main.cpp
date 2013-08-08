#include "videoplayer.h"
#include "sqhandlegstpath.h"
#include <QGst/Init>
#include <QGlib/Error>
#include <QGst/Message>
#include <QApplication>
#include <QDebug>
#include "sqarg.h"
#include <QMessageBox>
#include <iostream>

void    defineOption(SQArgDescMap &optionDesc);

void myMessageOutput(QtMsgType type, const char *msg)
{
    switch (type)
    {
        case QtDebugMsg:
            std::cerr << "Debug : " << msg << std::endl;
            break;
        case QtCriticalMsg:
            std::cerr << "Critical : " << msg << std::endl;
            qApp->exit(1);
            break;
        case QtWarningMsg:
            std::cerr << "Warning : " << msg << std::endl;
            break;
        case QtFatalMsg:
            std::cerr << "Fatal : " << msg << std::endl;
            break;
    }
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SQArgDescMap		optionDesc;
    QMap<QString, QVariant> option;

    if (argc < 2)
    {
        std::cerr << "Too few arguments, syntax is : -sub subfile videofile" << std::endl;
        return 1;
    }
    sq_add_gsttoyunda_plugin_path(qApp->applicationDirPath());
    try {
        QGst::init(&argc, &argv);
    } catch (QGlib::Error qgerr)
    {
        std::cerr << "Can't init gstreamer" << std::endl;
        return 1;
    }
    qInstallMsgHandler(myMessageOutput);
    defineOption(optionDesc);
    QStringList arg = a.arguments();
    arg.removeFirst();
    bool vopt = SQArg::fillWithDesc(option, arg, optionDesc);
    if (vopt == false)
        return 1;

    VideoPlayer w("");

    qDebug() << option["base_image_path"].toString();
    if (!option["base_image_path"].toString().isEmpty())
        w.setToyundaImagePath(option["base_image_path"].toString());
    std::cout << "Subtitle file : " << option["subtitle"].toString().constData() << std::endl;
    std::cout << "Video File    : " << arg[0].constData() << std::endl;
    w.setVideoFile(arg[0]);
    w.setSubFile(option["subtitle"].toString());
    w.show();
    w.play();
    w.show();
    return a.exec();
}

void	defineOption(SQArgDescMap &optionDesc)
{
    optionDesc["help"] = SQOpt("help", "h", false, "Show this help", "Show this help", false, true);
    optionDesc["videooutput"] = SQOpt("videooutput", "vo", "autovideosink", "The gstreamer videosink to use", "The gstreamer videosink to use", false, true);
    optionDesc["audiooutput"] = SQOpt("audiooutput", "ao", "autoaudiosink", "The Gstreamer audiosink", "The Gstreamer audiosink", false, true);
    optionDesc["fullscreen"] = SQOpt("fullscreen", "fs", false, "Set to fullscreen", "Set to fullscreen", false, true);
    optionDesc["base_image_path"] = SQOpt("base_image_path", "bip", QString(), "Set to image base path, usualy the karaoke dir", "Set to image base path, usualy the karaoke dir", false, true);
    optionDesc["logo"] = SQOpt("logo", "l", QString(), "Set the default image logo", "Set the default image logo", false, true);
    optionDesc["subtitle"] = SQOpt("subtitle", "sub", QString(), "The subtitle file", "The subtitle file", true);
}
