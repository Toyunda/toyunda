#ifndef PROFILMPLAYER_H
#define PROFILMPLAYER_H

#include <QString>
#include <QStringList>
#include <QProcess>
#include "graphicerrorhandler.h"
#include "profil.h"

class Profilmplayer : public Profil
{
public:
    Profilmplayer();
    void    setEH(GraphicErrorHandler*);

public slots:
    void    play(QString, QString);
    void    stop();
    bool    init();
    void    setVolume(int);
    void    dispose();
    void        on_finish(int);

 private:
    QString     m_mplayer_exec;
    QStringList m_mplayer_default_arg;
    QString     m_mplayer_exec_path;
    QProcess*    m_process;
};

#endif // PROFILMPLAYER_H
