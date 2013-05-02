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
