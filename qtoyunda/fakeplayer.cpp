#include <QDebug>
#include "fakeplayer.h"


FakePlayer::FakePlayer() : FilePlayer()
{
  optionDesc["interval"] = SQOpt("interval", "inter", QString("50"), "time in mlsec", "The time in mlsec between two tick", false);
}

bool	FakePlayer::init(const QStringList opt)
{
  setOption(opt, optionDesc);
  interval = optionValue["interval"];
  frameNumber = 0;
  return true;
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
