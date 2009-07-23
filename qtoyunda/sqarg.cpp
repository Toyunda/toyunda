/*
Copyright (C) 2008 - Sylvain Colinet
This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/
#include <QDebug>
#include <QRegExp>
#include "sqarg.h"

SQOpt::SQOpt(QString f, QString s, QVariant def, QString desc, QString l, bool m, bool sk) :
  fullName(f), shortName(s), defaultValue(def), description(desc), longHelp(l), mandatory(m),
  skillAll(sk)
{
  s_null = false;
}

SQOpt::SQOpt()
{
  s_null = true;
}

bool SQOpt::isNull() const
{
  return s_null;
}

bool	SQArg::findByShortName(QString optName, SQArgDescMap desc, SQOpt &opt)
{
  QMapIterator<QString, SQOpt> it(desc);
  while (it.hasNext()) {
    it.next();
    if (it.value().shortName == optName) {
      opt = it.value();
      return true;
    }
  }
  return false;
}

void	SQArg::generateShortHelp(SQArgDescMap desc)
{
  QTextStream cout(cout);
  QMapIterator<QString, QVariant> it(desc);
  while (it.hasNext()) {
    it.next();
    cout << it.value().longName << " : " << it.value().description << "\n";
  }
}

void	SQArg::generateLongHelp(SQArgDescMap desc)
{
  QTextStream cout(cout);
  QMapIterator<QString, QVariant> it(desc);
  while (it.hasNext()) {
    it.next();
    cout << it.value().longName << " : \n  " << it.value().longHelp << "\n";
  }
}

void	SQArg::fillWithDesc(QMap<QString, QVariant> &toFill, QStringList listOpt, SQArgDescMap desc)
{
  QListIterator<QString> i(listOpt);
  QRegExp  isLong("^--(.*)");
  QRegExp  isShort("^-([^-].*)");
  QRegExp  isLValue("(.*)=(.*)");

  QString optName;
  QString optValue;
  SQOpt opt;
  bool  shortOpt;
  bool  longOpt;
  while (i.hasNext()) {
    QString item = i.next();
    optValue.clear();
    longOpt = false;
    shortOpt = false;
    // Check -- or -
    if (isLong.exactMatch(item)) {
       optName = isLong.cap(1);
       longOpt = true;
    }
    else if (isShort.exactMatch(item)) {
       optName = isShort.cap(1);
       shortOpt = true;
    }
    else {
      qCritical() << "invalid usage of option";
      exit(1);
    }
    // check for opt=value form
    if (isLValue.exactMatch(optName)) {
      optName = isLValue.cap(1);
      optValue = isLValue.cap(2);
    }
    // fint the opt in desc
    if (shortOpt) {
      if (findByShortName(optName, desc, opt) == false) {
        qCritical() << "Can't find option" << optName;
        exit(1);
      }
     optName = opt.fullName;
    }
    if (longOpt) {
      if (desc.contains(optName))
        opt = desc[optName];
      else {
        qCritical() << "Can't find option" << optName;
        exit(1);
      }
    }
    // we have the opt!
    if (opt.defaultValue.type() == QVariant::Bool) {
      toFill[optName] = true;
      continue;
    }
    if (opt.skipAll)
      break;
    if (opt.defaultValue.canConvert(QVariant::String)) {
      if (optValue.isEmpty()) {
        if (i.hasNext())
          item = i.next();
        else {
          qCritical() << "Missing value for option : " << optName;
          exit(1);
        }
        if (isLong.exactMatch(item) or isShort.exactMatch(item)) {
          qCritical() << "Missing value for option : " << optName;
          exit(1);
        }
        toFill[optName] = item;
        continue;
      } else {
        toFill[optName] = optValue;
        continue;
      }
    }
  }
  // Check for mandatory options
  QMapIterator<QString, SQOpt> it(desc);
  while (it.hasNext()) {
    it.next();
    if (it.value().mandatory == true) {
      if (!toFill.contains(it.value().fullName)) {
        qCritical() << "Missing mandatory option " << it.value().fullName;
        exit(1);
      }
    } else {
      if (!toFill.contains(it.value().fullName))
        toFill[it.key()] = it.value().defaultValue;
    }
  }
}
