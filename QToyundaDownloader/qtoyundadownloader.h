#ifndef QTOYUNDADOWNLOADER_H
#define QTOYUNDADOWNLOADER_H

#include <QDialog>
#include <QStandardItem>
#include <QSettings>
#include <QProgressDialog>
#include <QFile>
#include <QNetworkReply>

#include <QNetworkAccessManager>
#include <QProcess>

namespace Ui {
class QToyundaDownloader;
}

class QToyundaDownloader : public QDialog
{
    Q_OBJECT
    
public:
    explicit QToyundaDownloader(QWidget *parent = 0);
    ~QToyundaDownloader();
    
private slots:
    void    httpFinished();
    void    httpReadyRead();
    void    updateDataReadProgress(qint64, qint64);
    void    cancelDownload();
    void    dezipProcessFinished(int exitCode, QProcess::ExitStatus qpExitStatus);

    void on_pathButton_clicked();

    void on_pushButton_clicked();

    void on_dlButton_clicked();

private:
    Ui::QToyundaDownloader *ui;

    bool    m_dlDone;
    bool    m_installDone;
    bool    m_abortRequest;

    QString m_7zipExec;
    QString m_dlPath;
    QString m_dezipPath;
    QSettings*   m_config;
    QFile*       m_file;

    QProgressDialog* m_progressDialog;
    QNetworkAccessManager   m_qnam;
    QNetworkReply*          m_reply;
    QStringList             m_toDL;
    QStringList             m_todezip;

    QStandardItem   m_qtoyItem;
    QStandardItem   m_mpItem;
    QStandardItem   m_toyToolItem;
    QStandardItem   m_rubItem;
    QStandardItem   m_gstItem;
    QProcess        *m_7zipProcess;

    void    createItem();
    void    downloadFile(QString, QUrl url);
    void    startRequest(QUrl url);
    void    decompressFile();
    void    nextDL();
    void    nextDeZip();
};

#endif // QTOYUNDADOWNLOADER_H
