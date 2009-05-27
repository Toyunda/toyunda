#ifndef H_TOYUNDASTREAM
#define H_TOYUNDASTREAM

#include <set>
#include <QString>
#include <QList>
#include <QObject>
#include "toyundasub.h"

class ToyundaSubStream : public QObject
{
  Q_OBJECT;
  public:
    ToyundaSubStream();
    ToyundaSubStream(QString);
    
    virtual QList<ToyundaText>  getCurrentTextSubtitle() const;
    virtual QList<ToyundaSyl>   getCurrentSylSubtitle() const;
    int				getCurrentFrame() const;
    QString			getIdentifier() const;
    virtual void		seek(const int);
    virtual void		createFromFile(const QString) = 0;

  public slots:
    virtual void	setCurrentFrame(const int);

  signals:
    virtual void	currentSubChanged();

  protected:
    QList<ToyundaText>  currentText;
    QList<ToyundaSyl>   currentSyl;
    int                 currentFrame;
    QString		identifier;
    std::set<ToyundaText> allText;
    std::set<ToyundaSyl>  allSyl;
    std::set<ToyundaText>::iterator currentItText;
    std::set<ToyundaSyl>::iterator currentItSyl;
};

#endif

