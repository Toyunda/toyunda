#ifndef H_PTOYUNDASUB
#define H_PTOYUNDASUB
#include <QString>
#include <QDebug>
#include <QTextStream>
#include <QColor>


const	unsigned int ToyundaHeight = 600;
const	unsigned int ToyundaWidth = 800;

struct ToyundaText {
  int		start;
  int		stop;
  int		pipeNumber;
  int		posx;
  int		posy;
  int		size;
  QString	text;
  QColor	color1;
  QColor	color2;
  QColor	fadingcolor;
};

struct ToyundaSyl {
  int	start;
  int	stop;
  int	pipeNumber;
  int	length;
  int	pos;
};

bool    operator<(const ToyundaText, const ToyundaText);
bool    operator<(const ToyundaSyl, const ToyundaSyl);

QDebug	operator<<(QDebug, const ToyundaText&);
QDebug	operator<<(QDebug, const ToyundaSyl&);

QDataStream	operator<<(QDataStream &, const ToyundaText);
//QTextStream	&operator<(QTextStream &, const ToyundaSyl);

QColor	toyundaColor2QColor(QString toycolor);

#endif
