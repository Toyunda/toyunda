#ifndef H_TOYUNDADRAWER
#define H_TOYUNDADRAWER

#include <QPainter>
#include "toyundasub.h"

class ToyundaDrawer
{
  public:
    ToyundaDrawer();
    void	draw(QPainter &painter, const QList<ToyundaText> &textSub, const QList<ToyundaSyl> &sylSub);
    void	setRatio(float nratio);
    void	setFont(QFont nFont);
    void	setLogo(QImage img);
  private:
    unsigned int	width;
    unsigned int	height;
    unsigned int	letterWidth;
    unsigned int	letterHeight;
    QImage		toyundaLogo;
    QFont		font;
    float	ratio;
};

#endif
