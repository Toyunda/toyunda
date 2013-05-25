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
    void    setErrorHandler(SQErrorHandler*);
    void    updateConfigDialog();
    void    updateValueFromDialog();
    bool    load(QString fileName);
    bool    save();

public slots:
    void    play(QString, QString);
    void    stop();
    bool    init();
    void    setVolume(int);
    void    dispose();
    void        on_finish(int);

 private:
    QString     m_mplayer_exec;
    QString     m_mplayer_additional_arg;
    QString     m_mplayer_WD;
    QString     m_mplayer_font_path;
    bool        m_fullscreen;
    QProcess*   m_process;

    friend class ProfilModel;
};

#endif // PROFILMPLAYER_H
