#ifndef H_TOYSTREAM
#define H_TOYSTREAM

#include <set>
#include "toytype.h"
#include <QString>
#include <QList>
#include <QObject>

class ToyStream : public QObject
{
  Q_OBJECT;
  public:
    ToyStream();

    void		loadFile(QString);
    QList<ToyText>	getCurrentText();
    QList<ToySyl>	getCurrentSyl();
    QColor		toycolor2QColor(QString);

  public slots:
    void	setCurrentFrame(int);
 
  signals:
    void	subChanged();

  private:
    QList<ToyText> currentText;
    QList<ToySyl> currentSyl;
    int		currentframe;
    std::set<ToyText> text;
    std::set<ToySyl> syl;

};

#endif
