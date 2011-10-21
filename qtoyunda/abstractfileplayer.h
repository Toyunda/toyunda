#ifndef ABSTRACTFILEPLAYER_H
#define ABSTRACTFILEPLAYER_H

#include "FilePlayer.h"

class AbstractFilePlayer
{
public:
    AbstractFilePlayer() = 0;
    FilePlayer  *getMe() = 0;
};

Q_DECLARE_INTERFACE(AbstractFilePlayer, "qtoyunda.AbstractFilePlayer")

#endif // ABSTRACTFILEPLAYER_H
