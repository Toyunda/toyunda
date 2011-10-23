#ifndef ABSTRACTFILEPLAYER_H
#define ABSTRACTFILEPLAYER_H

class FilePlayer;

class AbstractFilePlayer
{
public:
    virtual FilePlayer* getMe() = 0;
};

Q_DECLARE_INTERFACE(AbstractFilePlayer, "qtoyunda.AbstractFilePlayer")

#endif // ABSTRACTFILEPLAYER_H
