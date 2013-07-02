#include "qtoy2lyr.h"
#include "ui_qtoy2lyr.h"
#include <QFileDialog>
#include <QProcess>
#include <QMessageBox>
#include <QDebug>
#include "toyundagendialog.h"
#include "lyrsyntaxhighlighter.h"
#include "frmsyntaxhighlighter.h"

QToy2lyr::QToy2lyr(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QToy2lyr)
{
    ui->setupUi(this);
    m_settings = new QSettings("skarsnik.nyo.fr", "QToyunda");
    m_toyunda_tools = m_settings->value("toytooldir").toString();
    m_ruby_exec = m_settings->value("rubyexec").toString();
    m_toyunda_player = qApp->applicationDirPath() + "/qttoyunda-player";

    ui->infoLabel->setText("Using : " + m_toyunda_tools + " - ruby : " + m_ruby_exec + " - Player : " + m_toyunda_player);
    if (m_settings->contains("perlexec"))
        m_perl_exec = m_settings->value("perlexec").toString();
#ifdef Q_WS_X11
    if (m_perl_exec.isEmpty())
    {
        m_perl_exec = "/usr/bin/perl";
        ui->perlPathEdit->setText(m_perl_exec);
        m_settings->setValue("perlexec", m_perl_exec);
    }
#endif
    ui->perlPathEdit->setText(m_perl_exec);
    /*loadFile("K:/Lyrics/Slayers Next - OP - Give A Reason.txt");
    m_video = "K:/Videos/Slayers Next - OP - Give A Reason.avi";*/
    m_player = new QProcess(this);
    QStringList env = QProcess::systemEnvironment();
    env << "GST_PLUGIN_PATH=" + qApp->applicationDirPath() + "/gsttoyunda/";
    m_player->setEnvironment(env);
    m_player->setWorkingDirectory(qApp->applicationDirPath());

    LyrSyntaxHighlighter    *lyrSyn = new LyrSyntaxHighlighter(ui->lyrPlainTextEdit->document());
    frmSyntaxHighlighter    *frmSyn = new frmSyntaxHighlighter(ui->frmFileEdit->document());
}

QToy2lyr::~QToy2lyr()
{
    delete ui;
}


void QToy2lyr::on_perlChooseButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Choose the perl executable"), "", "");
    m_perl_exec = fileName;
    m_settings->setValue("perlexec", m_perl_exec);
    ui->perlPathEdit->setText(m_perl_exec);
}

void QToy2lyr::on_fileChooseButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Choose the file to work on it"), "" , "*.txt");
    loadFile(fileName);
}

void QToy2lyr::on_exectoy2lyrButton_clicked()
{
    if (ui->copyFileEdit->toPlainText().isEmpty() || m_perl_exec.isEmpty())
        return ;
    QString cpyPath = QDir::tempPath() + "/qtoy2lyr_cpy_" + QFileInfo(m_file).fileName();
    QFile f(cpyPath);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text))
        return;
    f.write(ui->copyFileEdit->toPlainText().toLatin1());
    f.close();
    QProcess    toy2lyrProc;
    toy2lyrProc.setWorkingDirectory(QDir::tempPath());
    QStringList args;
    args << m_toyunda_tools + "/toy2lyr.pl" << cpyPath;
    toy2lyrProc.start(m_perl_exec, args);
    toy2lyrProc.waitForFinished();
    QMessageBox mb;
    mb.setText(toy2lyrProc.readAll());
    mb.exec();
    if (toy2lyrProc.exitCode() != 0)
        return ;
    qDebug() << QFileInfo(cpyPath).baseName() + "_gen.lyr";
    f.setFileName(QDir::tempPath() + "/" + QFileInfo(cpyPath).baseName() + "_gen.lyr");
    if (! f.open(QIODevice::ReadOnly | QIODevice::Text))
        return ;
    ui->lyrPlainTextEdit->setPlainText(f.readAll());
    f.close();
    f.setFileName(QDir::tempPath() + "/" + QFileInfo(cpyPath).baseName() + "_gen.frm");
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text))
        return ;
    ui->frmFileEdit->setPlainText(f.readAll());
    f.close();
}

void QToy2lyr::loadFile(QString fileName)
{
    ui->filePathEdit->setText(fileName);
    if (!QFile::exists(fileName))
        return;
    m_file = fileName;
    QFile   f(fileName);
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text))
        return;
    ui->OriginalPlainTextEdit->setPlainText(f.readAll());
    ui->copyFileEdit->setPlainText(ui->OriginalPlainTextEdit->toPlainText());
}

void QToy2lyr::on_makeSubButton_clicked()
{
    ToyundaGenDialog toydiag(this);

    QStringList args;
    // Write lyr/frm file
    QString tmpLyr = QDir::tempPath() + "/" + QFileInfo(m_file).baseName() + "__.lyr";
    QString tmpFrm = QDir::tempPath() + "/" + QFileInfo(m_file).baseName() + "__.frm";
    m_tmpTxt = QDir::tempPath() + "/" + QFileInfo(m_file).baseName() + "__.txt";
    QFile f(tmpLyr);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text))
        return;
    f.write(ui->lyrPlainTextEdit->toPlainText().toLocal8Bit());
    f.close();
    f.setFileName(tmpFrm);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text))
        return;
    f.write(ui->frmFileEdit->toPlainText().toLocal8Bit());
    f.close();
    args << m_toyunda_tools + "/toyunda-gen.rb" << tmpLyr << tmpFrm;
    toydiag.execute(m_ruby_exec, args, m_tmpTxt, m_toyunda_tools);
}


void QToy2lyr::on_videoChooseButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Choose the file to work on it"), "" , "");
    m_video = fileName;
    ui->videoPathEdit->setText(m_video);
}

void QToy2lyr::on_launchButton_clicked()
{
    if (m_video.isEmpty())
    {
        QMessageBox::information(this, tr("No video set"), tr("You don't set the video file"));
        return;
    }
    m_player->setWorkingDirectory(qApp->applicationDirPath());
    QStringList arg;
    arg << "-sub" << m_tmpTxt << m_video;
    qDebug() << m_toyunda_player << arg;
    m_player->start(m_toyunda_player, arg);
}

void QToy2lyr::on_saveButton_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, "Choississez le répertoire pour sauvegarder", "", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (!dir.isEmpty())
    {
        QFile::copy(m_tmpTxt, dir + "/" + QFileInfo(m_file).fileName());
        QFile f(dir + "/" + QFileInfo(m_file).baseName() + ".lyr");
        f.open(QIODevice::WriteOnly | QIODevice::Text);
        f.write(ui->lyrPlainTextEdit->toPlainText().toLocal8Bit());
        f.close();
        f.setFileName(dir + "/" + QFileInfo(m_file).baseName() + ".frm");
        f.open(QIODevice::WriteOnly | QIODevice::Text);
        f.write(ui->frmFileEdit->toPlainText().toLocal8Bit());
        f.close();
    }
}
