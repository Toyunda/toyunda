#ifndef H_RAWSUBSTREAM
#define H_RAWSUBSTREAM

#include "toyundasubstream.h"

class RawSubStream : public ToyundaSubStream
{
  Q_OBJECT;
  public :
    RawSubStream();
    RawSubStream(const QString);
    void	createFromFile(const QString);
};


#endif
