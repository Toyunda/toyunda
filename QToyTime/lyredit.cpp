#include "lyredit.h"
#include "lyrsidebar.h"
#include <QPainter>
#include <QDebug>
#include <QTextBlock>

lyrEdit::lyrEdit(QWidget *parent) :
    QPlainTextEdit(parent)
{
    m_sideBar = new lyrSideBar(this);
    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateSideBarWidth(int)));
    connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateSideBar(QRect,int)));

    updateSideBarWidth(0);
    timedSel = false;
    currentSel = false;
    currentSyl = false;

    // Settings formats for selection
    timedPrevSylSel.format.setBackground(QBrush(QColor(210, 127, 237)));
    timedCurrentSylSel.format.setBackground(QBrush(Qt::green));
    preTimedSylSel.format.setBackground(QBrush(QColor(143, 87, 161)));
    currentSylSel.format.setBackground(QBrush(Qt::yellow));

    QBrush  lyrLineHighlightColor = palette().alternateBase();
    currentLineSel.format.setBackground(lyrLineHighlightColor);
    currentLineSel.format.setProperty(QTextFormat::FullWidthSelection, true);
}

void lyrEdit::sideBarPaintEvent(QPaintEvent *event)
{
    QPainter painter(m_sideBar);
    painter.fillRect(event->rect(), Qt::lightGray);
    painter.drawLine(m_sideBar->width() - 1, event->rect().top(), m_sideBar->width() - 1, event->rect().bottom());

    QTextBlock block = firstVisibleBlock();
    int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + (int) blockBoundingRect(block).height();

    while (block.isValid() && top <= event->rect().bottom())
    {
        if (block.isVisible() && bottom >= event->rect().top())
        {
            QString text = block.text();
            if (text.indexOf(QRegExp("[^\n#]+&[^\n]+")) != -1)
            {
                QString n = QString::number(text.count("&"));
                painter.setPen(Qt::black);
                painter.drawText(0, top, m_sideBar->width() - 3, fontMetrics().height(),
                             Qt::AlignRight, n);
            }
        }

        block = block.next();
        top = bottom;
        bottom = top + (int) blockBoundingRect(block).height();
    }
    if (markedBlock.isValid())
    {
        painter.setPen(Qt::black);
        //painter.drawText(0, blockBoundingGeometry(markedBlock).translated(contentOffset()).top(), m_sideBar->width(), fontMetrics().height(),
        //                 Qt::AlignLeft, "O");
        if (markImg.isNull())
        {
            QImage img(":/main/Toyunda logo.png");
            markImg = img.scaledToHeight(fontMetrics().height() - 3, Qt::SmoothTransformation);
        }
        QRectF rc = blockBoundingGeometry(markedBlock).translated(contentOffset());
        int posy = rc.top() + ((rc.bottom() - rc.top() - markImg.height()) / 2);
        painter.drawImage(0, posy, markImg);
    }
}

void lyrEdit::sideBarMousePressEvent(QMouseEvent *event)
{
    QTextBlock block = firstVisibleBlock();

    int blockNumber = block.blockNumber();
    int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + (int) blockBoundingRect(block).height();

    while (block.isValid() && bottom < event->y())
    {
        block = block.next();
        top = bottom;
        bottom = top + (int) blockBoundingRect(block).height();
        ++blockNumber;
    }
    if (block.isValid())
    {
        unSetTimedSelection();
        if (markedBlock.isValid() && blockNumber == markedBlock.blockNumber())
        {
            markedBlock = QTextBlock();
        }
        else
        {
            if (block.text().indexOf(QRegExp("[^\n#]+&[^\n]+")) != -1)
            {
                markedBlock = block;
            }
        }
    }
    update();
}


int lyrEdit::sideBarWidth()
{
    return fontMetrics().width('9') * 2 + 4;
}

void lyrEdit::setTimedSelection(int start, int prePrevSyl, int postPrevSyl, int preCurSyl, int posCurSel)
{
    timedSel = true;
    timedPrevSylSel.cursor = textCursor();
    timedPrevSylSel.cursor.setPosition(prePrevSyl);
    timedPrevSylSel.cursor.setPosition(postPrevSyl, QTextCursor::KeepAnchor);

    preTimedSylSel.cursor = textCursor();
    preTimedSylSel.cursor.setPosition(start);
    preTimedSylSel.cursor.setPosition(prePrevSyl, QTextCursor::KeepAnchor);

    timedCurrentSylSel.cursor = textCursor();
    timedCurrentSylSel.cursor.setPosition(preCurSyl);
    timedCurrentSylSel.cursor.setPosition(posCurSel, QTextCursor::KeepAnchor);
    setSelections();
}

void lyrEdit::unSetTimedSelection()
{
    timedSel = false;
    setSelections();
}

void lyrEdit::setCurrentSelection(int startLine, int startSyl, int stopSyl)
{
    currentSel = true;
    currentLineSel.cursor = textCursor();
    currentLineSel.cursor.setPosition(startLine);

    if (startSyl != -1)
    {
        currentSyl = true;
        currentSylSel.cursor = textCursor();
        currentSylSel.cursor.setPosition(startSyl);
        currentSylSel.cursor.setPosition(stopSyl, QTextCursor::KeepAnchor);
    }
    else
        currentSyl = false;
    setSelections();
}

void lyrEdit::unsetCurrentSelection()
{
    currentSel = false;
    setSelections();
}

const QTextBlock &lyrEdit::getMarkedBlock() const
{
    return markedBlock;
}

void    lyrEdit::resizeEvent(QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent(e);

    QRect cr = contentsRect();
    m_sideBar->setGeometry(QRect(cr.left(), cr.top(), sideBarWidth(), cr.height()));
}

void lyrEdit::updateSideBarWidth(int newBlockCount)
{
    Q_UNUSED(newBlockCount);
    setViewportMargins(sideBarWidth(), 0, 0, 0);
}

void lyrEdit::updateSideBar(const QRect &rec, int dy)
{
    if (dy)
        m_sideBar->scroll(0, dy);
    else
        m_sideBar->update(0, rec.y(), m_sideBar->width(), rec.height());
    if (rec.contains(viewport()->rect()))
        updateSideBarWidth(0);
}

void lyrEdit::setSelections()
{
    QList<QTextEdit::ExtraSelection> lsel;
    if (currentSel)
    {
        lsel.append(currentLineSel);
    }
    if (timedSel)
    {
        lsel.append(preTimedSylSel);
        lsel.append(timedCurrentSylSel);
        lsel.append(timedPrevSylSel);
    }
    if (currentSel && currentSyl)
        lsel.append(currentSylSel);
    setExtraSelections(lsel);
}
