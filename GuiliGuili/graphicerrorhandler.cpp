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


#include "graphicerrorhandler.h"
#include <QApplication>
#include <QDebug>
#include <QMessageBox>


GraphicErrorHandler::GraphicErrorHandler()
{
    qDebug() << "Create Graphic Error Handler";
}

void GraphicErrorHandler::addError(SQError err)
{
    if (err.level == SQError::Critical)
        qCritical() << err.message << err.info;
    if (err.level == SQError::Fatal)
        qCritical() << "FATAL : " << err.message << err.info;
    if (err.level == SQError::Warning)
        qWarning() <<  err.message << err.info;
    m_errorStack.append(err);
}

void GraphicErrorHandler::showError()
{
    QMessageBox msgBox;
    QString     msgText;
    QString     infoText;
    QString     detailText;


    msgText = "Une erreur est survenu";
    foreach(SQError err, m_errorStack)
    {
        infoText += err.message + "\n";
        detailText += err.info.toString();
    }
    msgBox.setText(msgText);
    msgBox.setInformativeText(infoText);
    msgBox.setDetailedText(detailText);
    msgBox.exec();
    m_errorStack.clear();
}
