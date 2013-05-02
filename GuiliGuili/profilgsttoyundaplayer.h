#ifndef GSTTOYUNDAPLAYERPROFIL_H
#define GSTTOYUNDAPLAYERPROFIL_H

#include "profil.h"
#include <QProcess>
#include <QObject>

class ProfilGstToyundaPlayer : public Profil
{
Q_OBJECT
public:
    ProfilGstToyundaPlayer();
    void    setErrorHandler(SQErrorHandler *);
    void    updateConfigDialog();
    void    updateValueFromDialog();
    bool    save();
    bool    load(QString fileName);


public slots:
    void    play(QString, QString);
    void    stop();
    bool    init();
    void    setVolume(int);
    void    dispose();
    void    on_finish(int);
    void    process_error(QProcess::ProcessError);

private:
    QProcess*   m_process;
    bool        m_scaled_fullscreen;
    bool        m_fullscreen;
    QString     m_videosink;
    QString     m_audiosink;
};

#endif // GSTTOYUNDAPLAYERPROFIL_H
