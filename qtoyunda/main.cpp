#include <QApplication>
#include <QObject>
#include <QTextStream>
QTextStream cout(stdout);
QTextStream cerr(stderr);

#include "toystream.h"
#include "qxine.h"
#include "qtoyosd.h"

int	main(int ac, char *ag[])
{
  QApplication app(ac, ag);
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
