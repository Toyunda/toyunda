#include "toyundasub.h"
#include "debugrender.h"

DebugRender::DebugRender()
{
  
}

void	DebugRender::renderUpdate()
{
  QList<ToyundaText> subText = toySubStream->getCurrentTextSubtitle();
  QList<ToyundaSyl> subSyl = toySubStream->getCurrentSylSubtitle();

  QTextStream cout(stdout);
  QTextStream cerr(stderr);
  if (subText.isEmpty()) {
    cout << "no current text to display\n";
  } else {
    cout << "----" << toySubStream->getCurrentFrame() << "----\n";
    QListIterator<ToyundaText> itText(subText);
    while (itText.hasNext())
    {
      cout << (ToyundaText) itText.next();
    }
  }
}
