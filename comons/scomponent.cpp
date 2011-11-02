/*
    This file is part of qtoyunda software
    Copyright (C) 2008  Sylvain "Skarsnik" Colinet <scolinet@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <QTextStream>
#include "scomponent.h"

SComponent::SComponent()
{
  //optionDesc["help"] = SQOpt("help", "help", false, "display help", "display help", false, true);
}

void	SComponent::handleOption(QStringList listArg)
{
  if (listArg.size() == 1 && listArg.at(0) == "")
    listArg.clear();
  if (optionDesc.isEmpty())
    return;
  listArg.replaceInStrings(QRegExp("^(.*)$"), "--\\1");
  SQArg::fillWithDesc(optionValue, listArg, optionDesc);
}

void	SComponent::addOption(QString name, QVariant defaultvalue, QString description, bool mandatory)
{
  optionDesc[name] = SQOpt(name, name, defaultvalue, description, description, mandatory, false);
}

void	SComponent::showOption()
{
  QTextStream cout(stdout);
  if (optionDesc.isEmpty()) {
   cout << "No option for " << identifier() << "\n";
   cout.flush();
  }
  else {
  cout << "Option for " << identifier() << " :\n";
  cout.flush();
  SQArg::generateShortHelp(optionDesc);
  }
}

QString	SComponent::identifier() const
{
  return m_identifier;
}

void	SComponent::setIdentifier(QString id)
{
  m_identifier = id;
}
