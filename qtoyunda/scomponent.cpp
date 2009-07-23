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

#include "sqarg.h"

SComponent::SComponent()
{
  optionDesc["help"] = SQOpt("help", "help", false, "display help", "display help", false, true);
}

void	SComponent::handleOption(QString listArg)
{
  if (optionDesc.isEmpty()) {
    qCritical() << "You must define optionDesc before call SComponent::handleOption";
    exit(1);
  }
  SQArg::fillWithDesc(optionValue, listArg, optionDesc);
}

void	SComponent::showOptionHelp()
{
  SQArg::generateLongHelp(optionDesc);
}

QString	SComponent::identifier() const
{
  return s_identifier;
}

void	SComponet::setIdentifier(QString id)
{
  s_identifier = id;
}