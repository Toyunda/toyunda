/*
    This file is part of QToyunda software
    Copyright (C) 2013  Sylvain "Skarsnik" Colinet <scolinet@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

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
#include "toyundagendialog.h"
#include <QMessageBox>

QToyTime::QToyTime(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::QToyTime)
{
    ui->setupUi(this);

    /*QToyTime::lineSylDesc *d;
    ui->lyrFileEdit->setPlainText("&il &fait &beau");
    ui->frmFileEdit->setPlainText("20 30");
    d = getLineSylDesc(false, true, 6, true);
    print_linedesc(*d);*/

    m_settings = new QSettings("skarsnik.nyo.fr", "QToyTime");

    ui->actionNew->setShortcut(QKeySequence::New);
    ui->actionOpen->setShortcut(QKeySequence::Open);
    ui->actionSave->setShortcut(QKeySequence::Save);
    ui->actionQuit->setShortcut(QKeySequence::Quit);
    ui->actionOpen->setIcon(style()->standardPixmap(QStyle::SP_DialogOpenButton));
    ui->actionSave->setIcon(style()->standardPixmap(QStyle::SP_DialogSaveButton));

    connect(ui->actionNew, SIGNAL(triggered()), this, SLOT(newTime()));
    connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(projectOpen()));
    connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(projectSave()));
    connect(ui->lyrFileEdit, SIGNAL(textChanged()), this, SLOT(onLyrTextChanged()));
    connect(ui->frmFileEdit, SIGNAL(textChanged()), this, SLOT(onFrmTextChanged()));
    connect(ui->lyrFileEdit, SIGNAL(cursorPositionChanged()), this, SLOT(onLyrCursorPositionChanged()));
    connect(ui->frmFileEdit, SIGNAL(cursorPositionChanged()), this, SLOT(onFrmCursorPositionChanged()));
    connect(ui->qpreviewButton, SIGNAL(clicked()), this, SLOT(on_actionQuickPreview_triggered()));


    addDockWidget(Qt::LeftDockWidgetArea, ui->lyrFileDock);
    addDockWidget(Qt::RightDockWidgetArea, ui->frmFileDock);
    addDockWidget( Qt::RightDockWidgetArea, ui->frameOutputDock);

    createToolBarFrmLyr();

    // settings gsttoy process
    m_gsttoyPlayerProcess = new QProcess();
    QStringList env = QProcess::systemEnvironment();
    env << "GST_PLUGIN_PATH=" + sq_get_gsttoyunda_plugin_path(qApp->applicationDirPath());
    m_gsttoyPlayerProcess->setEnvironment(env);
    m_gsttoyPlayerProcess->setStandardOutputFile(qApp->applicationDirPath() + "/toyunda-player.log");
    m_gsttoyPlayerProcess->setStandardErrorFile(qApp->applicationDirPath() + "/toyunda-player.log");
    m_gsttoyPlayerProcess->setWorkingDirectory(qApp->applicationDirPath());


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

    ui->player->videoWidget()->init(videosink);
    m_previewWindow->init(videosink);
    m_rawVideo = ui->player->videoWidget();

    connect(m_rawVideo, SIGNAL(videoMouseClickPress(int)), this, SLOT(onVideoMousePress(int)));
    connect(m_rawVideo, SIGNAL(videoMouseClickRelease(int)), this, SLOT(onVideoMouseRelease(int)));
    connect(ui->player, SIGNAL(playing()), SLOT(onVideoPlaying()));

    ui->statusBar->showMessage(tr("No Toyunda Time project loaded"));

    m_frameExtraSel.format.setBackground(QBrush(Qt::lightGray));


    // Settings
    if (m_settings->contains("windowGeometry"))
    {
        restoreGeometry(m_settings->value("windowGeometry").toByteArray());
        restoreState(m_settings->value("windowState").toByteArray());
    }

    QString warningMsg;
    if (m_settings->contains("rubyexec")  && !m_settings->value("rubyexec").toString().isEmpty())
    {
        m_rubyExec = m_settings->value("rubyexec").toString();
        m_configDialog.setRubyExec(m_rubyExec);
    }
    else
    {
        m_rubyExec = tryToFindRuby();
        if (!m_rubyExec.isEmpty())
        {
            m_configDialog.setRubyExec(m_rubyExec);
            m_settings->setValue("rubyexec", m_rubyExec);
        }
        else
        {
            if (!m_settings->contains("windowGeometry"))
                warningMsg = tr("no suitable ruby executable not found (ruby 1.8 or lower)\n");
            ui->fpreviewButton->setEnabled(false);
            ui->actionGenerateSubtitle->setEnabled(false);
            ui->actionFullPreview->setEnabled(false);
        }
    }
    if (m_settings->contains("toytooldir") && !m_settings->value("toytooldir").toString().isEmpty())
    {
        m_toyToolDir = m_settings->value("toytooldir").toString();
        m_configDialog.setToyToolDir(m_toyToolDir);
    }
    else
    {
#ifdef Q_WS_WIN32
        if (QFile::exists(qApp->applicationDirPath() + "/" + "toyunda-tools"))
        {
            m_toyToolDir = qApp->applicationDirPath() + "/" + "toyunda-tools";
            m_settings->setValue("toytooldir", m_toyToolDir);
        }
        else
#endif
        {
            if (!m_settings->contains("windowGeometry"))
                warningMsg += tr("Toyunda tools dir not found (searching for toyunda-tools directory in the application path).\n");
            ui->fpreviewButton->setEnabled(false);
            ui->actionGenerateSubtitle->setEnabled(false);
            ui->actionFullPreview->setEnabled(false);
        }
    }
    if(!warningMsg.isEmpty())
    {
        QMessageBox::warning(this, tr("ruby or Toyunda tools not found"), warningMsg +
        tr("QToyTime will not be able to generate the final toyunda subtitle. You can set this in the configuration dialog later.\n"
           "You can still use QToyTime to create the .lyr and .frm file.\n\n"
           "This warning will only be display once."));
    }
}

QToyTime::~QToyTime()
{
    delete ui;
}

void QToyTime::onVideoMousePress(int frameNb)
{
    int size = ui->frameOutputEdit->toPlainText().size();
    ui->frameOutputEdit->insertPlainText(QString().setNum(frameNb) + " ");
    if (m_cacheFrame.isEmpty())
    {
        m_frameExtraSel.cursor = ui->frameOutputEdit->textCursor();
        m_frameExtraSel.cursor.setPosition(size);
    }

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

void QToyTime::onVideoPlaying()
{
    clearCacheFrame();
}


void QToyTime::newTime()
{
    NewTimeDialog   diag;

//TODO : check if there is some change to save

    if (diag.exec())
    {
        m_time = new ToyundaTime();
        m_time->setBaseDir(diag.baseDir);
        m_time->setFrmFile(diag.frmFile);
        m_time->setLyrFile(diag.lyrFile);
        m_time->setIniFile(diag.iniFile);
        m_time->setVideoFile(diag.videoFile);
        m_time->setSubFile(diag.subFile);
        m_time->save();

        loadProject();
    }
}


void QToyTime::loadVideo()
{
    ui->player->setVideo(m_time->videoFile());
    m_previewWindow->setVideoFile(m_time->videoFile());
}


void QToyTime::projectOpen()
{
    QString iniFile = QFileDialog::getOpenFileName(this, tr("Choose Project File (ini)"),
                                                   "", "Ini file (*.ini)");
    if (!iniFile.isNull())
    {
        m_time = new ToyundaTime(iniFile);
        loadProject();
    }
}

void QToyTime::projectSave()
{
    m_time->save(ui->frmFileEdit->toPlainText(), ui->lyrFileEdit->toPlainText());
    setWindowModified(false);
    ui->lyrFileDock->setWindowModified(false);
    ui->frmFileDock->setWindowModified(false);
}

void QToyTime::quit()
{

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
    if (lineDesc != NULL && lineDesc->syls[lineDesc->indexCurrentSyl].hasFrm)
         highlightFrm(lineDesc);
    else
        ui->frmFileEdit->setExtraSelections(QList<QTextEdit::ExtraSelection>());
}

void QToyTime::onFrmCursorPositionChanged()
{
    QToyTime::lineSylDesc   *lineDesc;

    lineDesc = getLineSylDesc(false, true, ui->frmFileEdit->textCursor().position(), true);
    highlightFrm(lineDesc);
    if (lineDesc != NULL && lineDesc->syls[lineDesc->indexCurrentSyl].hasLyr)
        highlightLyrLine(lineDesc);
    else
        ui->lyrFileEdit->setExtraSelections(QList<QTextEdit::ExtraSelection>());
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
    if (&line == NULL)
    {
        qDebug() << "NULL linedesc";
        return;
    }
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
    QToyTime::SylDesc   currentSyl;

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
        currentSyl.hasLyr = true;
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
        while (cptSylBeforeLine < nbSylFrmCur && (index = lyrText.indexOf(lyrLineRegex, index)) >= 0)
        {
            startPosLyrLine = index;
            cptSylBeforeLine += lyrText.mid(index, lyrLineRegex.matchedLength()).count('&');
            index += lyrLineRegex.matchedLength();
            stopPosLyrLine = index;
        }
        cptSylBeforeLine -= lyrText.mid(startPosLyrLine, stopPosLyrLine - startPosLyrLine).count('&');
        if (index < 0)
        {
            currentSyl.hasLyr = false;
            startPosLyrLine = -1;
            stopPosLyrLine = -1;
        }
        else
        {
            lyrLine = lyrText.mid(startPosLyrLine, stopPosLyrLine - startPosLyrLine);
            currentSyl.hasLyr = true;
        }
    }

    int index = 0;
    if (currentSyl.hasLyr)
    {
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
    }
    if (fromLyr)
    {
        index = 0;
        while (nbSylLyrCur > nbSylFrmCur && (index = frmText.indexOf(frmLineRegex, index)) >= 0)
        {
            nbSylFrmCur++;
            startPosFrmLine = index;
            index += frmLineRegex.matchedLength();
            stopPosFrmLine = index;
        }
        if (index < 0)
        {
            currentSyl.hasFrm = false;
            startPosFrmLine = -1;
            stopPosFrmLine = -1;
        }
    }
    toret = new QToyTime::lineSylDesc;
    toret->posStartInLyrEdit = startPosLyrLine;
    toret->posEndInLyrEdit = stopPosLyrLine;
    toret->indexCurrentSyl = -1;

    currentSyl.posStartInFrmEdit = startPosFrmLine;
    currentSyl.posEndInFrmEdit = stopPosFrmLine;
    currentSyl.posStartInLyrEdit = startPosCurLyr;
    currentSyl.posEndInLyrEdit = stopPosCurLyr;
    currentSyl.myLine = toret;
    // Searching for all frame before cur
    if (currentSyl.hasFrm)
    {
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
    }
    toret->indexCurrentSyl = toret->syls.size();
    toret->syls.append(currentSyl);
    if (currentSyl.hasFrm)
    {
        //Searching for all frame after current
        int cptSyl = nbSylFrmCur;
        int nbTotalSylAtEnd = lyrLine.count('&') + cptSylBeforeLine;
        int posLyrEnd = currentSyl.posEndInLyrEdit;
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
    }
    toret->posStartInFrmEdit = toret->syls.first().posStartInFrmEdit;
    toret->posEndInFrmEdit = toret->syls.last().posEndInFrmEdit;
    //print_linedesc(*toret);
    return toret;
}


QString QToyTime::getLineUnderCursor(int pos, const QString &text, int *startpos, int *endpos)
{
    if (pos > text.size() ||
            (pos == 0 && text[pos] == '\n') ||
            (pos < text.size() && text[pos] == '\n' && text[pos-1] == '\n'))
        return QString("");
    else
    {
        int istartpos;
        if (pos > 0)
            pos--;
        while (pos > 0 && text[pos] != '\n')
            pos--;
        istartpos = pos + (pos == 0 ? 0 : 1);
        pos++;
        while (pos < text.size() && text[pos] != '\n')
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

    if (lineDesc != NULL)
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
    if (lineDesc != NULL && lineDesc->indexCurrentSyl != -1)
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
    return ;
    ui->dockWidgetContents->layout()->removeWidget(ui->lyrFileEdit);

    m_frmToolBar->addAction(style()->standardPixmap(QStyle::SP_DialogOpenButton), tr("&Open"), this, SLOT(frmOpen()));
    m_frmToolBar->addAction(style()->standardPixmap(QStyle::SP_DialogSaveButton), tr("&Save"), this, SLOT(frmSave()));

    m_lyrToolBar->addAction(style()->standardPixmap(QStyle::SP_DialogOpenButton), tr("&Open"), this, SLOT(lyrOpen()));
    m_lyrToolBar->addAction(style()->standardPixmap(QStyle::SP_DialogSaveButton), tr("&Save"), this, SLOT(lyrSave()));

    ui->dockWidgetContents->layout()->addWidget(m_lyrToolBar);
    ui->dockWidgetContents->layout()->addWidget(ui->lyrFileEdit);
    ui->dockWidgetContents_2->layout()->addWidget(m_frmToolBar);

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
        QTime pos = m_rawVideo->getTimeFromFrame(firstFrame);
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
        m_time = new ToyundaTime(m_settings->value("lastproject").toString());
        loadProject();
    }
}

void QToyTime::on_actionConfiguration_triggered()
{
    if (m_configDialog.exec())
    {
        m_settings->setValue("videosink", m_configDialog.videoSink);
        m_rubyExec = m_configDialog.rubyExec;
        if (!m_rubyExec.isEmpty())
            m_settings->setValue("rubyexec", m_configDialog.rubyExec);
        m_toyToolDir = m_configDialog.toyToolDir;
        if (!m_toyToolDir.isEmpty())
            m_settings->setValue("toytooldir", m_toyToolDir);
    }
}

void QToyTime::on_actionGenerateSubtitle_triggered()
{
    ToyundaGenDialog    diag;
    QStringList args;
    args << "toyunda-gen.rb" << QDir::currentPath() + "/" + m_time->lyrFile()
         << QDir::currentPath() + "/" + m_time->frmFile();
    qDebug() << m_toyToolDir;
    diag.execute(m_rubyExec, args, m_time->subFile(), m_toyToolDir);
}

void QToyTime::loadProject()
{
    m_settings->setValue("lastproject", m_time->baseDir() + "/" + m_time->iniFile());
    QDir::setCurrent(m_time->baseDir());
    m_time->loadLyrFrm();
    if (m_time->lyrFine())
        ui->lyrFileEdit->setText(m_time->getLyrText());
    if (m_time->frmFine())
        ui->frmFileEdit->setText(m_time->getFrmText());
    ui->lyrFileDock->setWindowTitle("[*]" + m_time->lyrFile());
    ui->frmFileDock->setWindowTitle("[*]" + m_time->frmFile());
    loadVideo();
    setWindowTitle("QToytime - [*]" + m_time->iniFile());
    ui->statusBar->showMessage(QString(tr("Project : %1 loaded")).arg(m_time->iniFile()), 10000);
    ui->lyrFileDock->setWindowModified(false);
    ui->frmFileDock->setWindowModified(false);
}

QString QToyTime::tryToFindRuby()
{
#ifdef Q_WS_X11
    QString path = qgetenv("PATH");
    QStringList pathList = path.split(':');
    foreach(const QString& mpath, pathList)
    {
        QString spath = mpath + "/ruby";
        if (QFile::permissions(spath) | QFile::ExeUser)
        {
            qDebug() << "testing ruby : " << spath;
            QProcess p;
            p.start(spath, QStringList() << "--version");
            p.waitForFinished(500);
            QString result = p.readAllStandardOutput();
            qDebug() << result;
            if (result.indexOf(QRegExp("ruby\\s+1.8")) != -1)
            {
                qDebug() << "Ruby 1.8.x found, nice";
                return spath;
            }
            else
                qDebug() << "Other version of ruby found, trying ruby1.8 executable";
        }
        spath = mpath + "/ruby1.8";
        if (QFile::permissions(spath) | QFile::ExeUser)
        {
            qDebug() << "find ruby1.8 executable, fine!";
            return spath;
        }
    }
#endif

#ifdef Q_WS_WIN32
    if (QFile::exists(qApp->applicationDirPath() + "/ruby"))
        return qApp->applicationDirPath() + "/ruby/bin/ruby.exe";
    if (QFile::exists(qApp->applicationDirPath() + "/ruby-1.8.7-p371-i386-mingw32"))
        return qApp->applicationDirPath() + "/ruby-1.8.7-p371-i386-mingw32/bin/ruby.exe";
#endif
    return QString();
}


void QToyTime::on_actionQuickPreview_triggered()
{
    QString tmpFile = QDir::tempPath() + "/Piko" + QString().setNum(QApplication::applicationPid()) + ".txt";
    if (generateToyundaSubtitle(ui->lyrFileEdit->toPlainText(), ui->frmFileEdit->toPlainText(), tmpFile))
    {
        m_gsttoyPlayerProcess->kill();
        QStringList arg;
        arg << QDir::currentPath().toUtf8() + "/"  + m_time->videoFile() << tmpFile;
        QString tmp = qApp->applicationDirPath();
        tmp.append("/toyunda-player");
        qDebug() << tmp << arg;
        m_gsttoyPlayerProcess->start(tmp, arg);
    }
}

void QToyTime::on_fpreviewButton_clicked()
{
    QString tmpLyr = QDir::tempPath() + "/Piko-lyr" + QString().setNum(QApplication::applicationPid()) + ".lyr";
    QString tmpFrm = QDir::tempPath() + "/Piko-FRM" + QString().setNum(QApplication::applicationPid()) + ".frm";
    QString tmpSub = QDir::tempPath() + "/Piko-gen" + QString().setNum(QApplication::applicationPid()) + ".txt";

    QFile fi(tmpLyr);
    fi.open(QIODevice::WriteOnly);
    fi.write(ui->lyrFileEdit->toPlainText().toLocal8Bit());
    fi.close();
    fi.setFileName(tmpFrm);
    fi.open(QIODevice::WriteOnly);
    fi.write(ui->frmFileEdit->toPlainText().toLocal8Bit());
    fi.close();
    QProcess    m_process;
    m_process.setWorkingDirectory(m_toyToolDir);
    m_process.setStandardOutputFile(tmpSub);
    m_process.start(m_rubyExec, QStringList() << m_toyToolDir + "/toyunda-gen.rb" << tmpLyr << tmpFrm);
    m_process.waitForFinished(2000);
    if (m_process.exitCode() != 0)
        QMessageBox::warning(this, tr("Can't generate full preview"), tr("Can't generate full preview, toyunda-gen.rb didn't work\n") + m_process.readAllStandardError());
    else
    {
        m_gsttoyPlayerProcess->kill();
        m_gsttoyPlayerProcess->start(qApp->applicationDirPath() + "/toyunda-player", QStringList() << m_time->videoFile() << tmpSub);
    }
}
