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
