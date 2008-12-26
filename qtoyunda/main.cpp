

#include <QApplication>
#include <QObject>
#include <QTextStream>
QTextStream cout(stdout);
QTextStream cerr(stderr);

#include "toystream.h"
#include "qxine.h"
#include "qtoyosd.h"

#ifdef Q_WS_X11
#include <X11/extensions/Xrender.h>
#endif

int	main(int ac, char *ag[])
{
#ifdef Q_WS_X11
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
    }


  QApplication app(dpy, ac, ag,
                     Qt::HANDLE(visual), Qt::HANDLE(colormap));
#elif
  QApplication app(ac, ag);
#endif
  cout << "parse toyunda file\n";
  ToyStream nm;
  nm.loadFile(QString(ag[1]));
  QXine p;
  p.init();
  cout << "starting play\n";
  QToyOSD osd;
  osd.setStream(&nm);
  p.start(QString(ag[2]));
  QObject::connect(&p, SIGNAL(frameChanged(int )), &nm, SLOT(setCurrentFrame(int)));
  QObject::connect(&nm, SIGNAL(subChanged()), &osd, SLOT(repaint()));
  osd.show();
  return app.exec();
  return 1;
}
