#include "lyrsidebar.h"

lyrSideBar::lyrSideBar(lyrEdit *le) :
    QWidget(le)
{
    m_lyrEdit = le;
}

QSize lyrSideBar::sizeHint() const
{
    return QSize(m_lyrEdit->sideBarWidth(), 0);
}

void lyrSideBar::paintEvent(QPaintEvent *ev)
{
    m_lyrEdit->sideBarPaintEvent(ev);
}

void lyrSideBar::mousePressEvent(QMouseEvent *ev)
{
    m_lyrEdit->sideBarMousePressEvent(ev);
}
