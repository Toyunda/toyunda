#ifndef H_PLOPT
#define H_PLOPT
#include <QString>
#include <QColor>

struct ToyText {
  int		start;
  int		stop;
  int		nbpipe;
  int		posx;
  int		posy;
  QString	text;
  QColor	color1;
  QColor	color2;
  QColor	tmpcolor;
};

struct ToySyl {
  int	start;
  int	stop;
  int	nbpipe;
  int	length;
  int	pos;
};

bool    operator<(ToyText, ToyText);
bool    operator<(ToySyl, ToySyl);
void	show_ToyText(ToyText);
void	show_ToySyl(ToySyl);


#endif
