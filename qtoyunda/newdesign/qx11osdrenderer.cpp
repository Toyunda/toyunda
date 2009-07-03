#include "qx11osdrenderer.h"
#include <QPainter>
#include <QPaintEvent>
#include <X11/extensions/Xrender.h>

QX11OSDRenderer::QX11OSDRenderer(QWidget *parent) : QWidget(parent), ToyundaRenderer()
{
  setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
  setAttribute(Qt::WA_NoSystemBackground, true);
  //setAutoFillBackground(true);
  //  /*QPixmap pix("fond.png");
  //    setMask(pix.mask());*/
  //      //setWindowOpacity(0.1);
  setFixedSize(QSize(ToyundaWidth, ToyundaHeight));
  identifier = "qx11osdrenderer";
}

QApplication *QX11OSDRenderer::init(int &ac, char *ag[])
{
  // Some black magic from QTlabs :)
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
  QApplication	*toret;
  toret = new QApplication(dpy, ac, ag,
                  Qt::HANDLE(visual), Qt::HANDLE(colormap));
  return toret;
}

void	QX11OSDRenderer::paintEvent(QPaintEvent *event)
{
  QPainter painter(this);
  // Magic from QtLabs.
  painter.setRenderHint(QPainter::Antialiasing);
  painter.setClipRect(event->rect());

  //make sure you clean your widget with a transparent
  //  color before doing any rendering
  //  note the usage of a composition mode Source
  //    it's important!
  painter.save();
  painter.setCompositionMode(QPainter::CompositionMode_Source);
  painter.fillRect(rect(), Qt::transparent);
  painter.restore();

  toyundaDrawer.draw(painter, toySubStream->getCurrentTextSubtitle(), toySubStream->getCurrentSylSubtitle());
}

void	QX11OSDRenderer::renderUpdate(void)
{
  update();
}
