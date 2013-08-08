#ifndef COLORSCHOOSERDIALOG_H
#define COLORSCHOOSERDIALOG_H

#include <QDialog>
#include <QLabel>

namespace Ui {
class ColorsChooserDialog;
}

class ColorsChooserDialog : public QDialog
{
    Q_OBJECT
    
    enum SylSel {
        post,
        pre,
        current
    };

public:
    explicit ColorsChooserDialog(QWidget *parent = 0);
    ~ColorsChooserDialog();
    
    static  QString toLyrString(const QColor& color, const QString& enc = "rgb");
    QColor currentSyl() const;
    void setCurrentSyl(const QColor &value);

    QColor postSyl() const;
    void setPostSyl(const QColor &value);

    QColor preSyl() const;
    void setPreSyl(const QColor &value);

    bool    eventFilter(QObject *obj, QEvent *event);

private slots:
    void    onCurrentColorChanged(const QColor&color);

private:
    Ui::ColorsChooserDialog *ui;

    QColor  m_currentSyl;
    QColor  m_postSyl;
    QColor  m_preSyl;
    int     syllableSel;

    void    setColor(int who, QColor color);
    void    setLabelColor(QLabel *lbl, const QColor& color);
};

#endif // COLORSCHOOSERDIALOG_H
