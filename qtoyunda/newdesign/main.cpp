#include <QApplication>
#ifdef Q_WS_X11
#include "qx11osdrenderer.h"
#endif
#include "toyundarenderer.h"
#include "toyundasubstream.h"
#include "debugrenderer.h"
#include "rawsubstream.h"
#include "fileplayer.h"
#include "fakeplayer.h"

int	main(int ac, char *ag[])
{
  FilePlayer		*player = NULL;
  RawSubStream		*substream;
  ToyundaRenderer	*renderer = NULL;
  QApplication		*qapp;
  QString		playerName = ag[1];
  QString		rendererName = ag[2];
  QString		subfile = ag[3];
  QString               videofile = ag[4];

  if (playerName == "fake")
    player = new FakePlayer;
  if (rendererName == "debug")
    renderer = new DebugRenderer;
#ifdef Q_WS_X11
  if (rendererName == "qx11osdrenderer")
    renderer = new QX11OSDRenderer;
#endif
  substream = new RawSubStream;
  qapp = renderer->init(ac, ag);
  player->init();
  QObject::connect(player, SIGNAL(frameChanged(int)), substream, SLOT(setCurrentFrame(int)));
  QObject::connect(substream, SIGNAL(currentSubChanged(void)), renderer, SLOT(renderUpdate(void)));
  renderer->setToyundaSubStream(substream);
  substream->createFromFile(subfile);
  player->open(videofile);
  player->play();
  return qapp->exec();
}
