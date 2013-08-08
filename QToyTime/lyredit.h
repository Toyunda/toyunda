#ifndef LYREDIT_H
#define LYREDIT_H

#include <QPlainTextEdit>
#include <QTextBlock>
#include <QCompleter>

class lyrEdit : public QPlainTextEdit
{
    Q_OBJECT
public:
    explicit lyrEdit(QWidget *parent = 0);

    void    sideBarPaintEvent(QPaintEvent* event);
    void    sideBarMousePressEvent(QMouseEvent* event);
    int     sideBarWidth();
    void    setTimedSelection(int start, int prePrevSyl, int postPrevSyl, int preCurSyl, int posCurSel);
    void    unSetTimedSelection();
    void    setCurrentSelection(int startLine, int startSyl = -1, int stopSyl = -1);
    void    unsetCurrentSelection();

    const QTextBlock& getMarkedBlock() const;

protected:
    void    resizeEvent(QResizeEvent *e);
     
signals:
    
    
private slots:
    void    updateSideBarWidth(int newBlockCount);
    void    updateSideBar(const QRect& rec, int);
    void    onModificationChanged(bool state);
    void    insertKeywordCompletion(const QString&);
    void    insertVariableCompletion(const QString&);
    void    onTextChanged();

private:
    QWidget*    m_sideBar;
    QImage      markImg;
    QTextBlock  markedBlock;
    QCompleter* lyrKeywordCompletion;
    QCompleter* lyrVariableCompletion;

    bool        currentSel;
    bool        currentSyl;
    bool        timedSel;

    QTextEdit::ExtraSelection   currentLineSel;
    QTextEdit::ExtraSelection   currentSylSel;
    QTextEdit::ExtraSelection   timedCurrentSylSel;
    QTextEdit::ExtraSelection   timedPrevSylSel;
    QTextEdit::ExtraSelection   preTimedSylSel;

    void        setSelections();
    void        initCompletion();
    void        insertCompletion(QCompleter *completer, QString toComp);
    void        keyPressEvent(QKeyEvent *e);

    QMap<QString, QString>  lyrKeywordDesc;
    QMap<QString, QList<QString> >    lyrVariable;
    QMap<QString, QString>  lyrVariableDesc;
};

#endif // LYREDIT_H
