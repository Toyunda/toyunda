/*
    This file is part of qtoyunda software
    Copyright (C) 2011  Sylvain "Skarsnik" Colinet <scolinet@gmail.com>

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


#ifndef GRAPHIC_ERRORHANDLER_H
#define GRAPHIC_ERRORHANDLER_H

#include "sqerrorhandler.h"

class GraphicErrorHandler : public SQErrorHandler
{
public:
    explicit GraphicErrorHandler();
    void    showError();
    void    addError(SQError);

private:
    QList<SQError>  m_errorStack;
};

#endif // ERRORHANDLER_H
