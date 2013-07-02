#ifndef QTOY2LYR_H
#define QTOY2LYR_H

#include <QWidget>
#include <QSettings>
#include <QProcess>

namespace Ui {
class QToy2lyr;
}

class QToy2lyr : public QWidget
{
    Q_OBJECT
    
public:
    explicit QToy2lyr(QWidget *parent = 0);
    ~QToy2lyr();
    
private slots:


    void on_perlChooseButton_clicked();

    void on_fileChooseButton_clicked();

    void on_exectoy2lyrButton_clicked();

    void on_makeSubButton_clicked();

    void on_videoChooseButton_clicked();

    void on_launchButton_clicked();

    void on_saveButton_clicked();

private:
    Ui::QToy2lyr *ui;
    QSettings*  m_settings;
    QString     m_toyunda_player;
    QString     m_toyunda_tools;
    QString     m_ruby_exec;
    QString     m_perl_exec;
    QString     m_file;
    QString     m_tmpTxt;
    QString     m_video;
    QProcess*    m_player;

    void    loadFile(QString fileName);
};

#endif // QTOY2LYR_H
