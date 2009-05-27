#include "toyundasub.h"

bool    operator<(const ToyundaText toy, const ToyundaText toy2)
{
  if (toy.start > toy2.start)
    return false;
  else if (toy.start == toy2.start)
  {
    if (toy.stop > toy2.stop)
      return false;
    else if (toy.stop < toy2.stop)
      return true;
    else
      return true;
  }
  else
    return true;
}

bool    operator<(const ToyundaSyl syl, const ToyundaSyl syl2)
{
  if (syl.start > syl2.start)
    return false;
  else if (syl.start == syl2.start)
  {
    if (syl.stop > syl2.stop)
      return false;
    else if (syl.stop < syl2.stop)
      return true;
    else
      return true;
  }
  else
    return true;
}

QDataStream	&operator<<(QDataStream &ts, const ToyundaText &tt)
{
  ;
}
/*
QTextStream	&operator<<(QTextStream &ts, const ToyundaSyl &tt)
{
  ;
}
*/
QDebug		operator<<(QDebug dbg, const ToyundaText &tt)
{
  dbg.nospace() << "Text : " << tt.text << "\n";
  dbg.nospace() << "Frame  : (" << tt.start << ", " << tt.stop << ")\n";
  dbg.nospace() << "Number of pipe : " << tt.pipeNumber << "\n";
  dbg.nospace() << "Position : (" << tt.posx << ", " << tt.posy << ")\n";
  dbg.nospace() << "Size : " << tt.size << "\n";
  dbg.nospace() << "Colors : -- " << tt.color1;
  if (tt.color2.isValid())
  {
    dbg.nospace() << " -- " << tt.color2 << " -- " << tt.fadingcolor;
  }
  return dbg;
}


QDebug		operator<<(QDebug dbg, const ToyundaSyl &ts)
{
  dbg.nospace() << "Frame  : (" << ts.start << ", " << ts.stop << ")\n";
  dbg.nospace() << "Number of pipe : " << ts.pipeNumber << "\n";
  dbg.nospace() << "Position : " << ts.pos << "\n";
}

// Enjoy the bgra format
QColor  toyundaColor2QColor(QString toycolor)
{
  QChar a, b;
  QString alp;
  qDebug() << toycolor;
  if (toycolor.size() == 8) {
    alp[0] = toycolor[0];
    alp[1] = toycolor[1];
    toycolor.remove(0, 2);
  }
  a = toycolor[0];
  b = toycolor[1];
  toycolor[0] = toycolor[4];
  toycolor[1] = toycolor[5];
  toycolor[4] = a;
  toycolor[5] = b;
  QColor toret("#" + toycolor);
  if (alp.isEmpty() == false)
    toret.setAlpha(alp.toInt((bool*) 0, 16));
  return toret;
}

