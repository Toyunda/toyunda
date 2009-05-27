#include "toyundasubstream.h"

ToyundaSubStream::ToyundaSubStream() : QObject()
{

}

ToyundaSubStream::ToyundaSubStream(const QString) : QObject()
{

}

void	ToyundaSubStream::seek(const int toseek)
{
  ;
}

int	ToyundaSubStream::getCurrentFrame() const
{
  return currentFrame;
}

QString	ToyundaSubStream::getIdentifier() const
{
  return identifier;
}

QList<ToyundaText>	ToyundaSubStream::getCurrentTextSubtitle() const
{
  return currentText;
}

QList<ToyundaSyl>	ToyundaSubStream::getCurrentSylSubtitle() const
{
  return currentSyl;
}

void	ToyundaSubStream::setCurrentFrame(const int cf)
{
  currentFrame = cf;
  bool	emitChange = false;

  // See if we have syl/text to remove
  QMutableListIterator<ToyundaSyl> i(currentSyl);
  while (i.hasNext()) {
    if (i.next().stop < currentFrame) {
      i.remove();
      emitChange = true;
    }
  }

  QMutableListIterator<ToyundaText> j(currentText);
  while (j.hasNext()) {
    ToyundaText &tmp = j.next();
    if (tmp.stop < currentFrame) {
      j.remove();
      emitChange = true;
    }
    else {
      if (tmp.color1.isValid() and tmp.color2.isValid()) {
        // Make Fading effect
	int r1, r2, g1, g2, b1, b2, a1, a2;
        #define MYABS(x) ((x) < 0 ? - (x) : (x))
	float rap = (float) (currentFrame - tmp.start) / (float) (tmp.stop - tmp.start);
	qDebug() << "before tmp : rap : " << rap << tmp.color1 << tmp.color2 << currentFrame;
	r1 = tmp.color1.red();
	r2 = tmp.color2.red();
	b1 = tmp.color1.blue();
	b2 = tmp.color2.blue();
	g1 = tmp.color1.green();
	g2 = tmp.color2.green();
	a1 = tmp.color1.alpha();
	a2 = tmp.color2.alpha();
	tmp.fadingcolor.setRed(r1 + rap * (r2 - r1));
	tmp.fadingcolor.setBlue(b1 + rap * (b2 - b1));
	tmp.fadingcolor.setGreen(g1 + rap * (g2 - g1));
	tmp.fadingcolor.setAlpha(a1 + rap * (a2 - a1));
	qDebug() << "after tmp" << tmp.fadingcolor;
	emitChange = true;
      }
    }
  }
  // update the currentSyl and currentText
  for (; currentItSyl != allSyl.end(); ++currentItSyl) {
    if (currentFrame < (*currentItSyl).start)
      break;
    emitChange = true;
    currentSyl << (*currentItSyl);
  }
  for (; currentItText != allText.end();++currentItText) {
    if (currentFrame < (*currentItText).start)
      break;
    emitChange = true;
    currentText << (*currentItText);
  }
  if (emitChange) {
    emit currentSubChanged();
  }
}
