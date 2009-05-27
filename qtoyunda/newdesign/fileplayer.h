#ifndef H_FILEPLAYER
#define H_FILEPLAYER

#include <QObject>
#include <QStringList>

class FilePlayer : public QObject
{
  Q_OBJECT;
  public:
    FilePlayer(QObject *parent = 0);
    virtual void	init(const QStringList) = 0;

  public slots:
    virtual void	open(const QString) = 0;
    virtual void	play() = 0;
    virtual void	stop() = 0;
    virtual void	seek(const int) = 0;

  signals:
    void	stopped();
    void	started();
    void	finished();
    void	frameChanged(int);
};

#endif
