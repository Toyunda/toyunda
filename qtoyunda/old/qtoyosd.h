#ifndef H_OSDT
#define H_OSDT

#include "toystream.h"
#include "toytype.h"
#include <QWidget>

class QToyOSD : public QWidget
{
  Q_OBJECT;
  public:
    QToyOSD(QWidget *parent = 0);
    void	setStream(ToyStream *);

  protected:
    void	paintEvent(QPaintEvent*);

  private:
    ToyStream	*toys;
    int		toyundaWidth;
    int		toyundaHeight;
    QSize	m_size;
    QImage	toylogo;
    float	ratio;
    int		fontSize;
    int		letterw;
    int		letterh;
    int		spacingSize;
};

#endif
