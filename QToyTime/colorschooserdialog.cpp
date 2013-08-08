#include "colorschooserdialog.h"
#include "ui_colorschooserdialog.h"
#include <QTextCharFormat>

ColorsChooserDialog::ColorsChooserDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ColorsChooserDialog)
{
    ui->setupUi(this);
    ui->colorChooser->setOption(QColorDialog::ShowAlphaChannel);
    ui->colorChooser->setOption(QColorDialog::NoButtons);
    ui->colorChooser->setOption(QColorDialog::DontUseNativeDialog);
    ui->colorChooser->setParent(this);
    ui->colorChooser->setWindowFlags(Qt::Widget);
    //ui->colorChooser->show();
    connect(ui->colorChooser, SIGNAL(currentColorChanged(QColor)), this, SLOT(onCurrentColorChanged(QColor)));
    ui->currentSylLabel->installEventFilter(this);
    ui->preSylLabel->installEventFilter(this);
    ui->postSylLabel->installEventFilter(this);
    m_currentSyl = QColor(0, 0xFF, 0);
    m_postSyl = QColor(0xFF, 0, 0);
    m_preSyl = QColor(0, 0, 0xFF);
    setLabelColor(ui->currentSylLabel, m_currentSyl);
    setLabelColor(ui->preSylLabel, m_preSyl);
    setLabelColor(ui->postSylLabel, m_postSyl);

    syllableSel = ColorsChooserDialog::pre;
    ui->whoLabel->setText(tr("Pre syllable color"));
    ui->colorChooser->setCurrentColor(m_preSyl);
}

ColorsChooserDialog::~ColorsChooserDialog()
{
    delete ui;
}

QString ColorsChooserDialog::toLyrString(const QColor &color, const QString &enc)
{
    if (enc == "rgb")
        return QString("%1%2%3%4").arg(color.red(), 0, 16).arg(color.green(), 0 , 16).arg(color.blue(), 0 , 16).arg(color.alpha(), 0, 16);
    else
        return QString("%1%2%3%4").arg(color.blue(), 0, 16).arg(color.green(), 0 , 16).arg(color.red(), 0 , 16).arg(color.alpha(), 0, 16);
}


void ColorsChooserDialog::onCurrentColorChanged(const QColor &color)
{
    setColor(syllableSel, color);
}

void ColorsChooserDialog::setColor(int who, QColor color)
{
    if (who == ColorsChooserDialog::pre)
    {
        setLabelColor(ui->preSylLabel, color);
        m_preSyl = color;
    }
    if (who == ColorsChooserDialog::post)
    {
        setLabelColor(ui->postSylLabel, color);
        m_postSyl = color;
    }
    if (who == ColorsChooserDialog::current)
    {
        setLabelColor(ui->currentSylLabel, color);
        m_currentSyl = color;
    }
    ui->lyrStringLabel->setText(QString("%color %1 %2 %3").arg(toLyrString(m_preSyl)).arg(toLyrString(m_currentSyl)).arg(toLyrString(m_postSyl)));
}

void ColorsChooserDialog::setLabelColor(QLabel *lbl, const QColor &color)
{
    lbl->setStyleSheet(QString("QLabel { color : rgba(%1, %2, %3, %4);}").arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alpha()));
}

bool ColorsChooserDialog::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonRelease)
    {
        if (obj == ui->currentSylLabel)
        {
            syllableSel = ColorsChooserDialog::current;
            ui->colorChooser->setCurrentColor(m_currentSyl);
            ui->whoLabel->setText(tr("Current syllable color"));
        }
        if (obj == ui->postSylLabel)
        {
            syllableSel = ColorsChooserDialog::post;
            ui->colorChooser->setCurrentColor(m_postSyl);
            ui->whoLabel->setText(tr("Post syllable color"));
        }
        if (obj == ui->preSylLabel)
        {
            syllableSel = ColorsChooserDialog::pre;
            ui->colorChooser->setCurrentColor(m_preSyl);
            ui->whoLabel->setText(tr("Pre syllable color"));
        }

    }
    return QObject::eventFilter(obj, event);
}


QColor ColorsChooserDialog::preSyl() const
{
    return m_preSyl;
}

void ColorsChooserDialog::setPreSyl(const QColor &value)
{
    m_preSyl = value;
    setLabelColor(ui->preSylLabel, value);
}


QColor ColorsChooserDialog::postSyl() const
{
    return m_postSyl;
}

void ColorsChooserDialog::setPostSyl(const QColor &value)
{
    m_postSyl = value;
    setLabelColor(ui->postSylLabel, value);
}

QColor ColorsChooserDialog::currentSyl() const
{
    return m_currentSyl;
}

void ColorsChooserDialog::setCurrentSyl(const QColor &value)
{
    m_currentSyl = value;
    setLabelColor(ui->currentSylLabel, value);
}
