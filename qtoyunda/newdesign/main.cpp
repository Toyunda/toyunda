#include <QApplication>
#ifdef Q_WS_X11
#include "qx11osdrenderer.h"
#endif
#include "toyundarenderer.h"
#include "toyundasubstream.h"
#include "debugrenderer.h"
#include "rawsubstream.h"
#include "fileplayer.h"
#include "fakeplayer.h"
#include <X11/extensions/Xrender.h>

int	main(int ac, char *ag[])
{
  FilePlayer		*player = NULL;
  RawSubStream		*substream;
  ToyundaRenderer	*renderer = NULL;
  QApplication		*qapp;
  QString		playerName = ag[1];
  QString		rendererName = ag[2];
  QString		subfile = ag[3];
  QString               videofile = ag[4];

  qDebug() << "Parsing command line argument";
  if (playerName == "fake")
    player = new FakePlayer;
  if (rendererName == "debug")
    renderer = new DebugRenderer;
#ifdef Q_WS_X11
  if (rendererName == "qx11osdrenderer") {
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
    renderer = new QX11OSDRenderer;
    renderer->init(ac, ag);
  } else {
    qapp = new QApplication(ac, ag);
  }
#endif
  substream = new RawSubStream;
  qDebug() << "Init player";
  player->init();
  qDebug() << "Connect signal/slots";
  QObject::connect(player, SIGNAL(frameChanged(int)), substream, SLOT(setCurrentFrame(int)));
  QObject::connect(substream, SIGNAL(currentSubChanged(void)), renderer, SLOT(renderUpdate(void)));
  renderer->setToyundaSubStream(substream);
  qDebug() << "Load subtitles";
  substream->createFromFile(subfile);
  qDebug() << "Open video";
  player->open(videofile);
  qDebug() << "Play the video";
  player->play();
  return qapp->exec();
}
