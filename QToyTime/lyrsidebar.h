#ifndef LYRSIDEBAR_H
#define LYRSIDEBAR_H

#include <QWidget>
#include "lyredit.h"

class lyrSideBar : public QWidget
{
    Q_OBJECT
public:
    explicit lyrSideBar(lyrEdit *lyrEdit = 0);
    QSize   sizeHint() const;
    
protected:
    void    paintEvent(QPaintEvent *ev);
    void    mousePressEvent(QMouseEvent *ev);

private:
    lyrEdit*    m_lyrEdit;
    
};

#endif // LYRSIDEBAR_H
