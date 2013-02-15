#ifndef PROFILOSD_H
#define PROFILOSD_H

#include "qtoyunda.h"
#include "profil.h"
#include "graphicerrorhandler.h"

class ProfilOSD : public Profil
{
public:
    ProfilOSD();
    void    setEH(GraphicErrorHandler*);

public slots:
    void    play(QString, QString);
    void    stop();
    bool    init();
    void    setVolume(int);
    void    dispose();

private:
    GraphicErrorHandler* m_errorHandler;
    QToyunda    *m_qtoyunda;
};

#endif // PROFILOSD_H
