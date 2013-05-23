#include "qtoytime.h"
#include "ui_qtoytime.h"
#include "newtimedialog.h"
#include <QFileInfo>
#include <QSettings>
#include <QDir>
#include <QPushButton>
#include "generatetoyundasubtitle.h"
#include <QProcess>
#include <QScrollBar>
#include <QFileDialog>
#include "lyrsyntaxhighlighter.h"
#include "frmsyntaxhighlighter.h"
#include "../comons/sqhandlegstpath.h"

QToyTime::QToyTime(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::QToyTime)
{
    ui->setupUi(this);

    m_settings = new QSettings("skarsnik.nyo.fr", "QToyTime");
    createCentralWidget();
    ui->actionNew->setShortcut(QKeySequence::New);
    ui->actionOpen->setShortcut(QKeySequence::Open);
    ui->actionSave->setShortcut(QKeySequence::Save);
    ui->actionQuit->setShortcut(QKeySequence::Quit);
    ui->actionNew->setIcon(style()->standardPixmap(QStyle::SP_FileIcon));
    ui->actionOpen->setIcon(style()->standardPixmap(QStyle::SP_DialogOpenButton));
    ui->actionSave->setIcon(style()->standardPixmap(QStyle::SP_DialogSaveButton));

    connect(ui->actionNew, SIGNAL(triggered()), this, SLOT(newTime()));
    connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(projectOpen()));
    connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(projectSave()));
    connect(ui->lyrFileEdit, SIGNAL(textChanged()), this, SLOT(onLyrTextChanged()));
    connect(ui->frmFileEdit, SIGNAL(textChanged()), this, SLOT(onFrmTextChanged()));
    connect(ui->lyrFileEdit, SIGNAL(cursorPositionChanged()), this, SLOT(onLyrCursorPositionChanged()));
    connect(ui->frmFileEdit, SIGNAL(cursorPositionChanged()), this, SLOT(onFrmCursorPositionChanged()));

    addDockWidget(Qt::LeftDockWidgetArea, ui->lyrFileDock);
    addDockWidget(Qt::RightDockWidgetArea, ui->frmFileDock);
    addDockWidget( Qt::RightDockWidgetArea, ui->frameOutputDock);
    /*ui->lyrFileEdit->resize(400, ui->lyrFileEdit->height());
    ui->frmFileEdit->resize(200, ui->frmFileEdit->height());*/
    createToolBarFrmLyr();
    m_process = new QProcess();
    m_process->setWorkingDirectory(qApp->applicationDirPath());
    QStringList env = QProcess::systemEnvironment();
    env << "GST_PLUGIN_PATH=" + sq_get_gsttoyunda_plugin_path(qApp->applicationDirPath());
    m_process->setEnvironment(env);
    m_process->setStandardOutputFile(qApp->applicationDirPath() + "/toyunda-player.log");
    m_process->setStandardErrorFile(qApp->applicationDirPath() + "/toyunda-player.log");
    m_process->setWorkingDirectory(qApp->applicationDirPath());
    LyrSyntaxHighlighter    *lyrSyn = new LyrSyntaxHighlighter(ui->lyrFileEdit->document());
    frmSyntaxHighlighter    *frmSyn = new frmSyntaxHighlighter(ui->frmFileEdit->document());

    m_previewWindow = new PreviewWindow(this);
    m_previewWindow->hide();
    QString videosink;
    if (m_settings->contains("videosink"))
    {
        m_configDialog.setVideoSink(m_settings->value("videosink").toString());
        videosink = m_settings->value("videosink").toString();
    }
    m_videoWidget->setFixedSize(480, 320);
    m_videoWidget->init(videosink);
    m_previewWindow->init(videosink);
    m_frameExtraSel.format.setBackground(QBrush(Qt::lightGray));

    if (m_settings->contains("windowGeometry"))
    {
        restoreGeometry(m_settings->value("windowGeometry").toByteArray());
        restoreState(m_settings->value("windowState").toByteArray());
    }
}

QToyTime::~QToyTime()
{
    delete ui;
}

void QToyTime::onVideoMousePress(int frameNb)
{
    if (m_cacheFrame.isEmpty())
    {
        m_frameExtraSel.cursor = ui->frameOutputEdit->textCursor();
        m_frameExtraSel.cursor.setPosition(ui->frameOutputEdit->toPlainText().size() - 1);
    }
    ui->frameOutputEdit->insertPlainText(QString().setNum(frameNb) + " ");
    m_cacheFrame.append(QString().setNum(frameNb) + " ");
}

void QToyTime::onVideoMouseRelease(int frameNb)
{
    QList<QTextEdit::ExtraSelection> sel;

    ui->frameOutputEdit->insertPlainText(QString().setNum(frameNb) + "\n");
    ui->frameOutputEdit->verticalScrollBar()->setValue(ui->frameOutputEdit->verticalScrollBar()->maximum());

    m_frameExtraSel.cursor.setPosition(ui->frameOutputEdit->toPlainText().size(), QTextCursor::KeepAnchor);
    sel.append(m_frameExtraSel);
    ui->frameOutputEdit->setExtraSelections(sel);
    m_cacheFrame.append(QString().setNum(frameNb) + "\n");


}

void QToyTime::onVideoWidgetKeyEvent(QKeyEvent ev)
{
    if (ev.key() == Qt::Key_Space)
    {
        if (m_videoWidget->state() == QGst::StatePlaying)
            m_videoWidget->pause();
        else
        {
            m_videoWidget->play();
            clearCacheFrame();
        }
    }
    if (ev.key() ==  Qt::Key_K)
    {
        ui->frameOutputEdit->insertPlainText("---\n");
        m_cacheFrame.clear();
    }
    if (ev.key() == Qt::Key_Right)
        m_videoWidget->setPosition(m_videoWidget->position().addSecs(20));
    if (ev.key() == Qt::Key_Left)
        m_videoWidget->setPosition(m_videoWidget->position().addSecs(-20));
}

void QToyTime::onVideoWidgetPositionChanged()
{
    QTime length(0,0);
    QTime curpos(0,0);

    if (m_videoWidget->state() != QGst::StateReady &&
            m_videoWidget->state() != QGst::StateNull)
    {
        length = m_videoWidget->duration();
        curpos = m_videoWidget->position();
    }
    if (length != QTime(0,0)) {
        m_posSlider->setValue(curpos.msecsTo(QTime()) * 1000 / length.msecsTo(QTime()));
    } else {
        m_posSlider->setValue(0);
    }
}

void QToyTime::newTime()
{
    NewTimeDialog   diag;

    if (diag.exec())
    {
        m_frmFile = diag.frmFile;
        m_lyrFile = diag.lyrFile;
        m_iniFile = diag.iniFile;
        m_videoFile = diag.videoFile;
        m_subFile = diag.subFile;
        m_settings->setValue("lastproject", m_iniFile);
        iniSave();
        QDir::setCurrent(diag.baseDir);
        frmOpen();
        lyrOpen();
        loadVideo();
        setWindowTitle("QToytime - [*]" + m_iniFile);
    }
}

void QToyTime::frmOpen()
{
    if (QFile::exists(m_frmFile))
    {
        ui->frmFileDock->setWindowTitle("[*]" + QFileInfo(m_frmFile).baseName());
        ui->frmFileDock->setWindowModified(false);
        QFile   fi(m_frmFile);
        fi.open(QIODevice::ReadOnly | QIODevice::Text);
        QString str(fi.readAll());
        fi.close();
        ui->frmFileEdit->setText(str);
        setWindowTitle("QToytime - [*]" + m_iniFile);
    }
}

void QToyTime::frmSave()
{
    QFile   fi(m_frmFile);

    fi.open(QIODevice::WriteOnly | QIODevice::Text);
    fi.write(ui->frmFileEdit->toPlainText().toLocal8Bit());
    fi.close();
    ui->frmFileDock->setWindowModified(false);
}

void QToyTime::lyrOpen()
{
    if (QFile::exists(m_lyrFile))
    {
        ui->lyrFileDock->setWindowTitle("[*]" + QFileInfo(m_lyrFile).fileName());
        ui->lyrFileDock->setWindowModified(false);
        QFile   fi(m_lyrFile);
        fi.open(QIODevice::ReadOnly | QIODevice::Text);
        QString str(fi.readAll());
        fi.close();
        ui->lyrFileEdit->setText(str);
    }
}

void QToyTime::lyrSave()
{
    QFile   fi(m_lyrFile);

    fi.open(QIODevice::WriteOnly | QIODevice::Text);
    fi.write(ui->lyrFileEdit->toPlainText().toLocal8Bit());
    fi.close();
    ui->lyrFileDock->setWindowModified(false);
}

void QToyTime::loadVideo()
{
    m_videoWidget->setVideoFile(m_videoFile);
    m_previewWindow->setVideoFile(m_videoFile);
    m_videoWidget->play();
    m_videoWidget->pause();
}

void QToyTime::iniOpen()
{
    QSettings    ini(m_iniFile, QSettings::IniFormat);
    m_videoFile = ini.value("movie/aviname").toString();
    m_lyrFile = ini.value("qtoytime/lyrfile").toString();
    m_frmFile = ini.value("qtoytime/frmfile").toString();
    m_subFile = ini.value("subtitles/file").toString();
}

void QToyTime::iniSave()
{
    QSettings   ini(m_iniFile, QSettings::IniFormat);
    ini.setValue("movie/aviname", QFileInfo(m_videoFile).fileName());
    ini.setValue("subtitles/file", QFileInfo(m_subFile).fileName());
    ini.setValue("qtoytime/lyrfile", QFileInfo(m_lyrFile).fileName());
    ini.setValue("qtoytime/frmfile", QFileInfo(m_frmFile).fileName());
}

void QToyTime::projectOpen()
{
    QString iniFile = QFileDialog::getOpenFileName(this, tr("Choose Project File (ini)"),
                                                   "", "Ini file (*.ini)");
    if (!iniFile.isNull())
    {
        m_iniFile = iniFile;
        m_settings->setValue("lastproject", m_iniFile);
        QDir::setCurrent(QFileInfo(m_iniFile).path());
        iniOpen();
        lyrOpen();
        frmOpen();
        setWindowModified(false);
        loadVideo();
    }
}

void QToyTime::projectSave()
{
    iniSave();
    frmSave();
    lyrSave();
    setWindowModified(false);
}

void QToyTime::quit()
{

}

void QToyTime::runPreview()
{
    QString tmpFile = QDir::tempPath() + "/Piko" + QString().setNum(QApplication::applicationPid()) + ".txt";
    if (generateToyundaSubtitle(ui->lyrFileEdit->toPlainText(), ui->frmFileEdit->toPlainText(), tmpFile))
    {
        m_process->kill();
        QStringList arg;
        arg << QDir::currentPath().toUtf8() + "/"  + m_videoFile << tmpFile;
        QString tmp = qApp->applicationDirPath();
        tmp.append("/toyunda-player");
        qDebug() << tmp << arg;
        m_process->start(tmp, arg);
    }
}

void QToyTime::onLyrTextChanged()
{
    ui->lyrFileDock->setWindowModified(true);
}

void QToyTime::onFrmTextChanged()
{
    ui->frmFileDock->setWindowModified(true);
}

void QToyTime::onLyrCursorPositionChanged()
{
    QToyTime::lineSylDesc   *lineDesc;

    lineDesc = getLineSylDesc(true, false, ui->lyrFileEdit->textCursor().position(), true);
    highlightLyrLine(lineDesc);
    if (lineDesc != NULL && lineDesc->indexCurrentSyl != -1)
         highlightFrm(lineDesc);
    else
        ui->frmFileEdit->setExtraSelections(QList<QTextEdit::ExtraSelection>());
}

void QToyTime::onFrmCursorPositionChanged()
{
    QToyTime::lineSylDesc   *lineDesc;

    lineDesc = getLineSylDesc(false, true, ui->frmFileEdit->textCursor().position(), true);
    highlightFrm(lineDesc);
    if (lineDesc != NULL)
        highlightLyrLine(lineDesc);
    else
        ui->lyrFileEdit->setExtraSelections(QList<QTextEdit::ExtraSelection>());
}

void QToyTime::onSliderMoved(int pos)
{
    if (m_videoWidget->state() != QGst::StateNull && m_videoWidget->state() != QGst::StateReady)
    {
        QTime tpos;
        qDebug() << pos;
        tpos = tpos.addMSecs((double) pos / 1000 * m_videoWidget->duration().msecsTo(QTime()) * -1);
        qDebug() << tpos;
        m_videoWidget->setPosition(tpos);
    }
}

void QToyTime::closeEvent(QCloseEvent *ev)
{
    m_settings->setValue("windowState", saveState());
    m_settings->setValue("windowGeometry", saveGeometry());
    QMainWindow::closeEvent(ev);
}

void QToyTime::print_syldesc(const QToyTime::SylDesc& desc)
{
    qDebug() << QString("\tLyrEdit : %1,%2 : $%3$").arg(desc.posStartInLyrEdit).arg(desc.posEndInLyrEdit)
                .arg(ui->lyrFileEdit->toPlainText().mid(desc.posStartInLyrEdit, desc.posEndInLyrEdit - desc.posStartInLyrEdit));
    qDebug() << QString("\tFRMEdit : %1,%2 : $%3$").arg(desc.posStartInFrmEdit).arg(desc.posEndInFrmEdit)
                                .arg(ui->frmFileEdit->toPlainText().mid(desc.posStartInFrmEdit, desc.posEndInFrmEdit - desc.posStartInFrmEdit));
    qDebug() << "\tStart : " << desc.frameStart << " Stop : " << desc.frameStop;
}

void QToyTime::print_linedesc(const QToyTime::lineSylDesc& line)
{
    qDebug() << QString("LyrLine : %1,%2 : %3$").arg(line.posStartInLyrEdit).arg(line.posEndInLyrEdit)
                .arg(ui->lyrFileEdit->toPlainText().mid(line.posStartInLyrEdit, line.posEndInLyrEdit - line.posStartInLyrEdit));
    foreach(const QToyTime::SylDesc& sylDesc, line.syls)
    {
        print_syldesc(sylDesc);
    }
}

void QToyTime::clearCacheFrame()
{
    m_cacheFrame.clear();
    ui->frameOutputEdit->setExtraSelections(QList<QTextEdit::ExtraSelection>());
}

QToyTime::lineSylDesc *QToyTime::getLineSylDesc(bool fromLyr, bool fromFrm, int posInEdit, bool posOnly)
{
    const   QString&    lyrText = ui->lyrFileEdit->toPlainText();
    const   QString&    frmText = ui->frmFileEdit->toPlainText();
    QString lyrLine;
    QString frmLine;
    QRegExp frmLineRegex("(\\d+) (\\d+)");
    QRegExp lyrLineRegex("([^\n#]+&[^\n]+)");
    QToyTime::lineSylDesc*  toret;

    int startPosLyrLine = -1;
    int stopPosLyrLine = -1;
    int startPosFrmLine = -1;
    int stopPosFrmLine = -1;
    int nbSylFrmCur = 0;
    int nbSylLyrCur = 0;
    int startPosCurLyr = -1;
    int stopPosCurLyr = -1;
    int cptSylBeforeLine = 0;

    if (fromLyr)
    {
        lyrLine = getLineUnderCursor(posInEdit, lyrText, &startPosLyrLine, &stopPosLyrLine);
        if (!lyrLineRegex.exactMatch(lyrLine))
            return NULL;
        // On compte le nombre de syl avant
        int index = 0;
        while ((index = lyrText.indexOf(lyrLineRegex, index)) >= 0 && index < startPosLyrLine)
        {
            cptSylBeforeLine += lyrText.mid(index, lyrLineRegex.matchedLength()).count('&');
            index += lyrLineRegex.matchedLength();
        }
    }
    if (fromFrm)
    {
        frmLine = getLineUnderCursor(posInEdit, frmText, &startPosFrmLine, &stopPosFrmLine);
        if (!frmLineRegex.exactMatch(frmLine))
            return NULL;
        int index = 0;
        // On compte le nombre de syl
        while ((index = frmText.indexOf(frmLineRegex, index)) >= 0 && index <= startPosFrmLine)
        {
            nbSylFrmCur++;
            index += frmLineRegex.matchedLength();
        }
        // On cherche la ligne associée
        index = 0;
        while ((index = lyrText.indexOf(lyrLineRegex, index)) >= 0 && cptSylBeforeLine < nbSylFrmCur)
        {
            startPosLyrLine = index;
            cptSylBeforeLine += lyrText.mid(index, lyrLineRegex.matchedLength()).count('&');
            index += lyrLineRegex.matchedLength();
            stopPosLyrLine = index;
        }
        cptSylBeforeLine -= lyrText.mid(startPosLyrLine, stopPosLyrLine - startPosLyrLine).count('&');
        if (index < 0)
            qDebug() << "no lyr line";
        else
        {
            lyrLine = lyrText.mid(startPosLyrLine, stopPosLyrLine - startPosLyrLine);

        }
    }

    int index = 0;
    // On cherche la position de la syl courante dans le lyrEdit
    nbSylLyrCur = cptSylBeforeLine;
    while ((index = lyrLine.indexOf('&', index)) >= 0)
    {
        if (fromFrm && nbSylLyrCur >= nbSylFrmCur)
            break;
        if (fromLyr && startPosLyrLine + index > posInEdit)
            break;
        startPosCurLyr = startPosLyrLine + index;
        index++;
        nbSylLyrCur++;
    }
    if (index >= 0)
        stopPosCurLyr = startPosLyrLine + index;
    else
        stopPosCurLyr = stopPosLyrLine;
    if (fromLyr)
    {
        index = 0;
        while ((index = frmText.indexOf(frmLineRegex, index)) >= 0 && nbSylLyrCur > nbSylFrmCur)
        {
            nbSylFrmCur++;
            startPosFrmLine = index;
            index += frmLineRegex.matchedLength();
            stopPosFrmLine = index;
        }
    }
    toret = new QToyTime::lineSylDesc;
    toret->posStartInLyrEdit = startPosLyrLine;
    toret->posEndInLyrEdit = stopPosLyrLine;
    toret->indexCurrentSyl = -1;
    QToyTime::SylDesc   currentSyl;
    currentSyl.posStartInFrmEdit = startPosFrmLine;
    currentSyl.posEndInFrmEdit = stopPosFrmLine;
    currentSyl.posStartInLyrEdit = startPosCurLyr;
    currentSyl.posEndInLyrEdit = stopPosCurLyr;
    currentSyl.myLine = toret;
    // Searching for all frame before cur
    index = startPosFrmLine - 1;
    int cptSyl = nbSylFrmCur;
    int posLyrEnd = startPosCurLyr - 1;
    QRegExp altFrmRegex("\\b(\\d+) (\\d+)");
    while ((index = frmText.lastIndexOf(altFrmRegex, index)) >= 0 && (cptSylBeforeLine  + 1) < cptSyl)
    {
        cptSyl--;
        QToyTime::SylDesc   newSyl;
        newSyl.frameStart = altFrmRegex.cap(1).toInt();
        newSyl.frameStop = altFrmRegex.cap(2).toInt();
        newSyl.posStartInFrmEdit = index;
        newSyl.posEndInFrmEdit = index + altFrmRegex.matchedLength();
        index -= altFrmRegex.matchedLength();
        newSyl.posStartInLyrEdit = lyrText.lastIndexOf('&', posLyrEnd - 1);
        newSyl.posEndInLyrEdit = posLyrEnd;
        posLyrEnd = newSyl.posStartInLyrEdit;
        toret->syls.prepend(newSyl);
    }
    currentSyl.frameStart = frmLineRegex.cap(1).toInt();
    currentSyl.frameStop = frmLineRegex.cap(2).toInt();
    toret->indexCurrentSyl = toret->syls.size();
    toret->syls.append(currentSyl);
    //Searching for all frame after current
    cptSyl = nbSylFrmCur;
    int nbTotalSylAtEnd = lyrLine.count('&') + cptSylBeforeLine;
    posLyrEnd = currentSyl.posEndInLyrEdit;
    QRegExp sylRegex("&[^&\n]+");
    index = currentSyl.posEndInFrmEdit;
    while ((index = frmText.indexOf(frmLineRegex, index)) >= 0 && cptSyl < nbTotalSylAtEnd)
    {
        QToyTime::SylDesc   newSyl;
        newSyl.frameStart = frmLineRegex.cap(1).toInt();
        newSyl.frameStop = frmLineRegex.cap(2).toInt();
        newSyl.posStartInFrmEdit = index;
        newSyl.posEndInFrmEdit = index + frmLineRegex.matchedLength();
        newSyl.posStartInLyrEdit = lyrText.indexOf(sylRegex, posLyrEnd);
        newSyl.posEndInLyrEdit = newSyl.posStartInLyrEdit + sylRegex.matchedLength();
        posLyrEnd = newSyl.posEndInLyrEdit;
        index += frmLineRegex.matchedLength();
        toret->syls.append(newSyl);
        cptSyl++;
    }
    toret->posStartInFrmEdit = toret->syls.first().posStartInFrmEdit;
    toret->posEndInFrmEdit = toret->syls.last().posEndInFrmEdit;
    //print_linedesc(*toret);
    return toret;
}

QString QToyTime::getLineUnderCursor(int pos, const QString &text, int *startpos, int *endpos)
{
    if (pos >= text.size() || (pos == 0 && text[pos] == '\n') || (text[pos] == '\n' && text[pos-1] == '\n'))
        return QString("");
    else
    {
        int istartpos;
        if (pos > 0)
            pos--;
        while (text[pos] != '\n' && pos > 0)
            pos--;
        istartpos = pos + (pos == 0 ? 0 : 1);
        pos++;
        while (text[pos] != '\n' && pos < text.size())
            pos++;
        if (startpos != NULL)
            *startpos = istartpos;
        if (endpos != NULL)
            *endpos = pos;
        //qDebug() << startpos << pos - startpos;
        return text.mid(istartpos, pos - istartpos);
    }
}

void QToyTime::highlightLyrLine(QToyTime::lineSylDesc *lineDesc)
{
    QList<QTextEdit::ExtraSelection> lyrExtraSelections;
    QTextEdit::ExtraSelection   lineSelection;
    QBrush  lyrLineHighlightColor = ui->lyrFileEdit->palette().alternateBase();
    lineSelection.format.setBackground(lyrLineHighlightColor);
    lineSelection.format.setProperty(QTextFormat::FullWidthSelection, true);
    lineSelection.cursor = ui->lyrFileEdit->textCursor();
    lyrExtraSelections.append(lineSelection);

    if (lineDesc != NULL && lineDesc->indexCurrentSyl != -1)
    {
        QTextEdit::ExtraSelection   sylSel;
        sylSel.cursor = ui->lyrFileEdit->textCursor();
        sylSel.cursor.setPosition(lineDesc->syls[lineDesc->indexCurrentSyl].posStartInLyrEdit);
        sylSel.cursor.setPosition(lineDesc->syls[lineDesc->indexCurrentSyl].posEndInLyrEdit, QTextCursor::KeepAnchor);
        //sylSel.cursor.clearSelection();
        sylSel.format.setBackground(QBrush(Qt::yellow));
        lyrExtraSelections.append(sylSel);
    }
    ui->lyrFileEdit->setExtraSelections(lyrExtraSelections);
}

void QToyTime::highlightFrm(QToyTime::lineSylDesc *lineDesc)
{
    QList<QTextEdit::ExtraSelection>    extraSels;
    if (lineDesc != NULL)
    {
        QTextCursor cur(ui->frmFileEdit->document());
        QTextEdit::ExtraSelection   frmCurSel;
        frmCurSel.cursor = cur;
        frmCurSel.format.setBackground(QBrush(Qt::yellow));
        frmCurSel.cursor.setPosition(lineDesc->syls[lineDesc->indexCurrentSyl].posStartInFrmEdit);
        frmCurSel.cursor.setPosition(lineDesc->syls[lineDesc->indexCurrentSyl].posEndInFrmEdit, QTextCursor::KeepAnchor);


        QTextEdit::ExtraSelection   frmLineSel;
        frmLineSel.cursor = cur;
        frmLineSel.format.setBackground(ui->lyrFileEdit->palette().mid());
        frmLineSel.cursor.setPosition(lineDesc->posStartInFrmEdit);
        frmLineSel.cursor.setPosition(lineDesc->posEndInFrmEdit, QTextCursor::KeepAnchor);
        frmLineSel.format.setProperty(QTextFormat::FullWidthSelection, true);

        extraSels.append(frmLineSel);
        extraSels.append(frmCurSel);
    }
    else
    {
        QTextEdit::ExtraSelection   lineSelection;
        QBrush  frmLineHighlightColor = ui->frmFileEdit->palette().alternateBase();
        lineSelection.format.setBackground(frmLineHighlightColor);
        lineSelection.format.setProperty(QTextFormat::FullWidthSelection, true);
        lineSelection.cursor = ui->lyrFileEdit->textCursor();
        extraSels.append(lineSelection);
    }
    ui->frmFileEdit->setExtraSelections(extraSels);
}



void QToyTime::createToolBarFrmLyr()
{
    m_frmToolBar = new QToolBar();
    m_lyrToolBar = new QToolBar();

    m_frmToolBar->addAction(style()->standardPixmap(QStyle::SP_DialogOpenButton), tr("&Open"), this, SLOT(frmOpen()));
    m_frmToolBar->addAction(style()->standardPixmap(QStyle::SP_DialogSaveButton), tr("&Save"), this, SLOT(frmSave()));

    m_lyrToolBar->addAction(style()->standardPixmap(QStyle::SP_DialogOpenButton), tr("&Open"), this, SLOT(lyrOpen()));
    m_lyrToolBar->addAction(style()->standardPixmap(QStyle::SP_DialogSaveButton), tr("&Save"), this, SLOT(lyrSave()));

    ui->dockWidgetContents->layout()->addWidget(m_lyrToolBar);
    ui->dockWidgetContents_2->layout()->addWidget(m_frmToolBar);

}

void QToyTime::createCentralWidget()
{
    QGridLayout*    gl = new QGridLayout();

    m_posSlider = new QSlider(Qt::Horizontal);
    m_posSlider->setTickPosition(QSlider::TicksBelow);
    m_posSlider->setTickInterval(10);
    m_posSlider->setMaximum(1000);

    QWidget*    wid = new QWidget();
    m_videoWidget = new VideoWidget();

    gl->addWidget(m_videoWidget, 0, 0, 1, 2);
    gl->addWidget(m_posSlider, 1, 0, 1, 2);
    QPushButton *previewB = new QPushButton();
    previewB->setText("Preview");
    gl->addWidget(previewB, 2, 0);
    wid->setLayout(gl);

    connect(previewB, SIGNAL(clicked()), this, SLOT(runPreview()));
    connect(m_videoWidget, SIGNAL(videoMouseClickPress(int)), this, SLOT(onVideoMousePress(int)));
    connect(m_videoWidget, SIGNAL(videoMouseClickRelease(int)), this, SLOT(onVideoMouseRelease(int)));
    connect(m_videoWidget, SIGNAL(keyEvent(QKeyEvent)), this, SLOT(onVideoWidgetKeyEvent(QKeyEvent)));
    connect(m_videoWidget, SIGNAL(positionChanged()), this, SLOT(onVideoWidgetPositionChanged()));
    connect(m_posSlider, SIGNAL(sliderMoved(int)), this, SLOT(onSliderMoved(int)));

    wid->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    setCentralWidget(wid);
}




void QToyTime::on_actionPreview_Last_Frame_Input_triggered()
{
    QString tmpFile = QDir::tempPath() + "/Piko-PrevFrame" + QString().setNum(QApplication::applicationPid()) + ".txt";
    if (!m_cacheFrame.isEmpty())
    {
        qDebug() << m_cacheFrame;
        generateToyundaSubtitle(ui->lyrFileEdit->toPlainText(), ui->frmFileEdit->toPlainText() + "\n" + m_cacheFrame, tmpFile);

        int firstFrame = m_cacheFrame.split('\n').first().split(' ').first().toInt();
        qDebug() << "Firstframe : " << firstFrame;
        QTime pos = m_videoWidget->getTimeFromFrame(firstFrame);
        pos = pos.addSecs(-3);
        qDebug() << "seek to : " << pos;
        qDebug() << tmpFile;
        m_previewWindow->setSubFile(tmpFile);
        m_previewWindow->show();
        m_previewWindow->setPosition(pos);
        m_previewWindow->play();
    }
}

void QToyTime::on_actionInsert_last_frame_input_triggered()
{
    if (!m_cacheFrame.isEmpty())
    {
        QTextCursor cur = ui->frmFileEdit->textCursor();
        cur.movePosition(QTextCursor::End, QTextCursor::MoveAnchor, 1);
        cur.insertText("\n" + m_cacheFrame);
    }
}

void QToyTime::on_actionOpen_last_Project_triggered()
{
    if (m_settings->contains("lastproject"))
    {
        m_iniFile = m_settings->value("lastproject").toString();
        qDebug() << m_iniFile;
        QDir::setCurrent(QFileInfo(m_iniFile).path());
        iniOpen();
        frmOpen();
        lyrOpen();
        loadVideo();
    }
}

void QToyTime::on_actionConfiguration_triggered()
{
    if (m_configDialog.exec())
    {
        m_settings->setValue("videosink", m_configDialog.videoSink);
    }
}
