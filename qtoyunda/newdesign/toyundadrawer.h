#ifndef H_TOYUNDADRAWER
#define H_TOYUNDADRAWER

#include <QPainter>
#include "toyundasub.h"

class ToyundaDrawer
{
  public:
    ToyundaDrawer();
    void	draw(QPainter &painter, const QList<ToyundaText> &textSub, const QList<ToyundaSyl> &sylSub) const;
    void	setRatio(const float hRat, const float vRat);
    void	setRatio(const float ratio);
    void	setFont(const QFont nFont);
    void	setLogo(const QImage img);
  private:
    unsigned int	width;
    unsigned int	height;
    unsigned int	letterWidth;
    unsigned int	letterHeight;
    QImage		toyundaLogo;
    QFont		font;
    float	horizontalRatio;
    float	verticalRatio;
};

#endif
