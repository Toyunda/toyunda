#include <QApplication>
#include <QList>
#include <QDataStream>
#include "rawsubstream.h"
#include "testbase.h"

template<typename T>QTextStream &operator<<(QTextStream &s, const QList<T> &l) {
    s << quint32(l.size());
    for (int i = 0; i < l.size(); ++i)
        s << l.at(i);
    return s;
}

using namespace testclass;

int	main(int ac, char *ag[])
{
  ToyundaText		testtext;
  ToyundaSyl		testsyl;
  QList<ToyundaText>	lt;
  QList<ToyundaSyl>	ls;
  RawSubStream		sst;

  ac = ac;
  ag = ag;
  testtext.start = 1;
  testtext.stop = 100;
  testtext.pipeNumber = 0;
  testtext.posx = -1;
  testtext.posy = -1;
  testtext.size = -1;
  testsyl.start = 1;
  testsyl.stop = 100;
  testsyl.pipeNumber = 1;
  testsyl.length = 2;
  testsyl.pos = 1;

  inittest("Basic sub one line, one syl");
  sst.createFromFile("001_testbasic.txt");
  sst.setCurrentFrame(2);
  testtext.text = "test basic";
  lt << testtext;
  ls << testsyl;
  is_equal(sst.getCurrentTextSubtitle(), lt);
  is_equal(sst.getCurrentSylSubtitle(), ls);
  return conclude();
}
