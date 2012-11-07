#ifndef PROFIL_H
#define PROFIL_H

#include <QObject>
#include <QDialog>

class Profil : public QObject
{
    Q_OBJECT

public:
	enum	BaseType {
		OSD,
		MPLAYER
	};
public:
    explicit Profil(QObject *parent = 0);
    QString	name;
    QString	description;
    QDialog	*configDialog;
    BaseType	baseType;

signals:
    void    played();
    void    paused();
    void    finished();
    void    stopped();
public slots:
    virtual void    play(QString, QString) = 0;
    virtual void    stop() = 0;
    virtual bool    init() = 0;
    virtual void    setVolume(int) = 0;
    virtual void    dispose() = 0;
};

#endif // PROFIL_H
