#include <QDebug>
#include "fakeplayer.h"


FakePlayer::FakePlayer() : FilePlayer()
{
  setIdentifier("fake");
  addOption("interval", QString("50"), "The time in mlsec between two tick");
}

bool	FakePlayer::init(const QStringList opt)
{
  handleOption(opt);
  interval = optionValue["interval"].toInt();
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
