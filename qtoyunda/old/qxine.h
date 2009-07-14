#ifndef H_NYOT
#define H_NYOT
#include <QString>
#include <QObject>
#include <xine.h>
#include <xine/xineutils.h>

class	QXine : public QObject
{
  Q_OBJECT;
  public:
    QXine();
    void	init();
    void	start(QString);

  signals:
    void	frameChanged(int );

  private slots:
    void	checkFrame();

  private:
    xine_t              *xine;
    xine_stream_t       *stream;
    int			duree_moisie;
};

#endif
