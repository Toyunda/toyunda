/*
    This file is part of GuiliGuili software
    Copyright (C) 2013  Sylvain "Skarsnik" Colinet <scolinet@gmail.com>

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

#ifndef PROFIL_H
#define PROFIL_H

#include <QObject>
#include <QDialog>
#include <QSettings>
#include "../comons/sqerrorhandler.h"

class Profil : public QObject
{
    Q_OBJECT

public:
	enum	BaseType {
		OSD,
		MPLAYER,
		GSTPLAYER
	};
public:
    explicit Profil(QObject *parent = 0);
    virtual void        setErrorHandler(SQErrorHandler *) = 0;
    QString         name;
    QString         description;
    QDialog*        configDialog;
    QString         fileName;
    BaseType        baseType;
    virtual void    updateConfigDialog() = 0;
    virtual void    updateValueFromDialog() = 0;
    virtual bool    save() = 0;
    virtual bool    load(QString fileName) = 0;
    bool            isInitialised() const;
    static          const QString     ProfilDirectory;
protected:
    SQErrorHandler* m_errorHandler;
    bool            m_initialised;
    void            baseSave(QSettings *iniFile);
    void            baseLoad(QSettings *iniFile);

signals:
    void    played();
    void    paused();
    void    finished();
    void    stopped();
    void    error_occured();
    void    fatal_error_occured();
public slots:
    virtual void    play(QString, QString) = 0;
    virtual void    stop() = 0;
    virtual bool    init() = 0;
    virtual void    setVolume(int) = 0;
    virtual void    dispose() = 0;
};


#endif // PROFIL_H
