/*
    This file is part of QToyunda software
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

#ifndef TOYUNDATIME_H
#define TOYUNDATIME_H

#include <QString>

class ToyundaTime
{
public:
    ToyundaTime();
    ToyundaTime(QString ini);
    bool    save(const QString &frmText = QString(), const QString &lyrText = QString());
    bool    loadLyrFrm();
    QString getLyrText() const;
    QString getFrmText() const;


    QString iniFile() const;
    void setIniFile(const QString &iniFile);

    QString frmFile() const;
    void setFrmFile(const QString &frmFile);

    QString lyrFile() const;
    void setLyrFile(const QString &lyrFile);

    QString videoFile() const;
    void setVideoFile(const QString &videoFile);

    QString subFile() const;
    void setSubFile(const QString &subFile);

    bool    lyrFine() const;
    bool    frmFine() const;

    QString baseDir() const;
    void setBaseDir(const QString &baseDir);

private:
    QString m_frmText;
    QString m_lyrText;

    QString m_iniFile;
    QString m_frmFile;
    QString m_lyrFile;
    QString m_videoFile;
    QString m_subFile;
    QString m_baseDir;

    bool    m_lyrFine;
    bool    m_frmFine;
};

#endif // TOYUNDATIME_H
