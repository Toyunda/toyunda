#include <QApplication>
#include "qtoyunda.h"
#include <X11/extensions/Xrender.h>

int	main(int ac, char *ag[])
{
  QToyunda              *toyunda;
  QApplication		*qapp;

  qDebug() << "Parsing command line argument";
  if (ac != 5) {
    qCritical() << "Invalid arguments, syntax : qtoyunda playername renderername video subtitle";
    exit(1);
  }
  QString		playerName = ag[1];
  QString		rendererName = ag[2];
  QString               videofile = ag[3];
  QString		subfile = ag[4];

  toyunda = new QToyunda(playerName, rendererName);
// Special Initialisation for the qosd renderer on old QT 4.x version
#ifdef Q_WS_X11
#if QT_VERSION < 0x040500
  if (rendererName == "qosd") {
    qWarning("Please make sure you're running a composition manager!");
    bool  argbVisual=false;
    Display *dpy = XOpenDisplay(0); // open default display
    if (!dpy) {
          qWarning("Cannot connect to the X server");
          exit(1);
    }

    int screen = DefaultScreen(dpy);
    Colormap colormap = 0;
    Visual *visual = 0;
    int eventBase, errorBase;

    if (XRenderQueryExtension(dpy, &eventBase, &errorBase)) {
          int nvi;
          XVisualInfo templ;
          templ.screen  = screen;
          templ.depth   = 32;
          templ.c_class = TrueColor;
          XVisualInfo *xvi = XGetVisualInfo(dpy, VisualScreenMask |
                                          VisualDepthMask |
                                          VisualClassMask, &templ, &nvi);

          for (int i = 0; i < nvi; ++i) {
          XRenderPictFormat *format = XRenderFindVisualFormat(dpy,
                                                                  xvi[i].visual);
          if (format->type == PictTypeDirect && format->direct.alphaMask) {
                  visual = xvi[i].visual;
                  colormap = XCreateColormap(dpy, RootWindow(dpy, screen),
                                          visual, AllocNone);
                  argbVisual=true;
                  break;
          }

          }
    }
    if (argbVisual == true) {
          qWarning("Found ARGB visual. Starting app...");
    }
    else  {
          qWarning("Couldn't find ARGB visual... Exiting.");
          exit(1);
    }
    qapp = new QApplication(dpy, ac, ag,
                  Qt::HANDLE(visual), Qt::HANDLE(colormap));
  } else {
#endif
#endif
    qapp = new QApplication(ac, ag);
#ifdef Q_WS_X11
#if QT_VERSION < 0x040500
  }
#endif
#endif

  qDebug() << "Init toyunda";
  toyunda->init();
  qDebug() << "Load files";
  toyunda->load(videofile, subfile);
  qDebug() << "Play the video";
  toyunda->play();
  return qapp->exec();
}
