#include <QApplication>
#include <QObject>
#include <QTextStream>
#include "fakeplayer.h"
#include "rawsubstream.h"
#include "debugrender.h"

int	main(int ac, char *ag[])
{
  QApplication app(ac, ag);
  FakePlayer fp;
  DebugRender dr;
  QString tickInter("50");

  if (ac != 2 and ac != 3) {
    QTextStream cerr(stderr);
    cerr << "Argument required, syntax :\n" << ag[0] << " subfile [tick Interval]\n";
    return 1;
  }
  if (ac == 3)
    tickInter = ag[2];
  RawSubStream ts(ag[1]);

  QObject::connect(&fp, SIGNAL(frameChanged(int)), &ts, SLOT(setCurrentFrame(int)));
  QObject::connect(&ts, SIGNAL(currentSubChanged(void)), &dr, SLOT(renderUpdate(void)));
  dr.setToyundaSubStream(&ts);
  QStringList l;
  l << tickInter;
  fp.init(l);
  fp.play();
  return app.exec();
}
