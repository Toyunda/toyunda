#include <QTextStream>
#include <QDebug>
#include <iostream>
#include "toytype.h"
extern QTextStream cout;

bool    operator<(ToyText toy, ToyText toy2)
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

bool    operator<(ToySyl syl, ToySyl syl2)
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

void    show_ToyText(ToyText t)
{
  std::cout << "start : " << t.start << "\n";
  std::cout << "stop : " << t.stop << "\n";
  std::cout << "text : " << t.text.toStdString() << "\n";
  std::cout << "nbpipe : " << t.nbpipe << "\n";
  qDebug() << "Color : " << t.color1;
  qDebug() << "Color : " << t.color2;
}

void    show_ToySyl(ToySyl t)
{
  std::cout << "start : " << t.start << "\n";
  std::cout << "stop : " << t.stop << "\n";
  std::cout << "pos : " << t.pos << "\n";
  std::cout << "length : " << t.length << "\n";
  std::cout << "nbpipe : " << t.nbpipe << "\n";
}

