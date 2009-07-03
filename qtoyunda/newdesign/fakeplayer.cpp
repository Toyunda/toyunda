#include <QDebug>
#include "fakeplayer.h"


FakePlayer::FakePlayer() : FilePlayer()
{
}

void	FakePlayer::init(const QStringList opt)
{
  if (opt.empty())
    interval = 50;
  else
    interval = opt[0].toInt();
  frameNumber = 0;
}

void	FakePlayer::init(const unsigned int interv)
{
  interval = interv;
  frameNumber = 0;
}

void	FakePlayer::open(const QString file)
{
  
}

void	FakePlayer::seek(const int toseek)
{
  frameNumber += toseek;
  emit frameChanged(frameNumber);
}

void	FakePlayer::play()
{
  connect(&timer, SIGNAL(timeout()), this, SLOT(newTick()));
  emit started();
  timer.start(interval);
}

void	FakePlayer::stop()
{
  timer.stop();
  emit stopped();
}

void	FakePlayer::newTick()
{
  emit frameChanged(frameNumber++);
}
