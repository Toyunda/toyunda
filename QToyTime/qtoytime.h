#ifndef QTOYTIME_H
#define QTOYTIME_H

#include <QMainWindow>
#include "videowidget.h"
#include <QSlider>
#include <QProcess>
#include "previewwindow.h"
#include <QSettings>
#include "configdialog.h"
#include <QTextEdit>
#include "qgplayer.h"
#include "toyundatime.h"

namespace Ui {
class QToyTime;
}

class QToyTime : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit QToyTime(QWidget *parent = 0);
    ~QToyTime();
    
private slots:
    void    onVideoMousePress(int);
    void    onVideoMouseRelease(int);
    void    onVideoPlaying();
    void    newTime();
    void    projectOpen();
    void    projectSave();
    void    quit();
    void    onLyrTextChanged();
    void    onFrmTextChanged();
    void    onLyrCursorPositionChanged();
    void    onFrmCursorPositionChanged();
    void    closeEvent(QCloseEvent *ev);


    void on_actionPreview_Last_Frame_Input_triggered();

    void on_actionInsert_last_frame_input_triggered();

    void on_actionOpen_last_Project_triggered();

    void on_actionConfiguration_triggered();

    void on_actionGenerateSubtitle_triggered();

    void on_actionQuickPreview_triggered();

private:
    struct  SylDesc;
    struct  lineSylDesc
    {
        QList<SylDesc>  syls;
        int indexCurrentSyl;
        int posStartInLyrEdit;
        int posEndInLyrEdit;
        int posStartInFrmEdit;
        int posEndInFrmEdit;
    };

    struct  SylDesc
    {
        int frameStart;
        int frameStop;
        int posStartInFrmEdit;
        int posEndInFrmEdit;
        int posStartInLyrEdit;
        int posEndInLyrEdit;
        bool    hasFrm;
        bool    hasLyr;
        lineSylDesc*    myLine;
    };

    Ui::QToyTime *ui;

    QGPlayer*       m_qgplayer;
    VideoWidget*    m_rawVideo;


    QToolBar*       m_lyrToolBar;
    QToolBar*       m_frmToolBar;
    QProcess*       m_gsttoyPlayerProcess;

    ToyundaTime*    m_time;
    PreviewWindow*  m_previewWindow;
    QSettings*      m_settings;
    configDialog    m_configDialog;

    bool    m_modifiedFrm;
    bool    m_modifiedLyr;
    QString m_cacheFrame;
    QTextEdit::ExtraSelection m_frameExtraSel;

    QString m_rubyExec;
    QString m_toyToolDir;


    void            loadProject();
    QString         tryToFindRuby();

    lineSylDesc*    getLineSylDesc(bool fromLyr, bool fromFrm, int  posInEdit, bool posOnly);
    QString         getLineUnderCursor(int  pos, const QString& text, int* startpos = NULL, int* endpos = NULL);
    void    createToolBarFrmLyr();
    void    createCentralWidget();
    void    highlightLyrLine(lineSylDesc *lineDesc);
    void    highlightFrm(lineSylDesc *lineDesc);

    void print_syldesc(const QToyTime::SylDesc& desc);
    void print_linedesc(const QToyTime::lineSylDesc& line);

    void    clearCacheFrame();

    void    loadVideo();

};

#endif // QTOYTIME_H
