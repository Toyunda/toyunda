#ifndef GSTTOYUNDAPLAYERPROFIL_H
#define GSTTOYUNDAPLAYERPROFIL_H

#include "profil.h"
#include <QProcess>

class ProfilGstToyundaPlayer : public Profil
{
public:
    ProfilGstToyundaPlayer();
    void    setErrorHandler(SQErrorHandler *);
    void    updateConfigDialog();
    void    updateValueFromDialog();


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
};

#endif // GSTTOYUNDAPLAYERPROFIL_H
