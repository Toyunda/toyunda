#ifndef H_FAKEPLAYER
#define H_FAKEPLAYER

#include <QTimer>
#include "fileplayer.h"

class FakePlayer : public FilePlayer
{
  Q_OBJECT;
  public:
    FakePlayer();

    void	init(const QStringList opt);
    void	init(const unsigned int interv);

  public slots:
    void	open(const QString);
    void	play();
    void	stop();
    void	seek(const int);

  private slots:
    void	newTick();

  private:
    unsigned int interval;
    QTimer	timer;
    int		frameNumber;
};

#endif
