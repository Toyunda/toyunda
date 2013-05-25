#include "qtoyundadownloader.h"
#include "ui_qtoyundadownloader.h"
#include <QDir>
#include <QDebug>
#include <QFileDialog>
#include <QStandardItem>
#include <QMessageBox>

QToyundaDownloader::QToyundaDownloader(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QToyundaDownloader)
{
    ui->setupUi(this);
    ui->pathEdit->setText(QDir::currentPath());
    m_dlPath = QDir::currentPath();
    ui->tableView->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
    createItem();
    m_config = new QSettings(":/new/configDL.ini", QSettings::IniFormat);
    m_progressDialog = new QProgressDialog();
    m_progressDialog->hide();
    m_abortRequest = false;

    QSettings key7zip("HKEY_CURRENT_USER\\Software\\7-Zip",
                        QSettings::NativeFormat);
    m_7zipExec = key7zip.value("Path").toString() + "\\" + "7z.exe";

    connect(m_progressDialog, SIGNAL(canceled()), this, SLOT(cancelDownload()));
}

QToyundaDownloader::~QToyundaDownloader()
{
    delete ui;
}

void QToyundaDownloader::on_pathButton_clicked()
{
    QString dirPath = QFileDialog::getExistingDirectory(this, "Choississez le répertoire de téléchargement", "", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (!dirPath.isEmpty())
    {
        m_dlPath = dirPath;
        ui->pathEdit->setText(dirPath);
    }
}

static  void    setItemProperty(QStandardItem& it)
{
    it.setCheckable(true);
    it.setCheckState(Qt::Checked);
    it.setEditable(false);
}

void QToyundaDownloader::createItem()
{
    QStandardItemModel* model = new QStandardItemModel();
    QStandardItem*  item;
    QList<QStandardItem*> listItem;
    QStringList listDesc;
    QStringList listTitle;

    model->setColumnCount(2);
    ui->tableView->setModel(model);

    setItemProperty(m_qtoyItem);
    setItemProperty(m_rubItem);
    setItemProperty(m_mpItem);
    setItemProperty(m_toyToolItem);
    setItemProperty(m_gstItem);

    listItem << &m_qtoyItem << &m_toyToolItem << &m_mpItem << &m_gstItem << &m_rubItem;
    listTitle << "QToyunda" << "Toyunda Tools" << "MPlayer Toyunda" << "GStreamer SDK" << "Ruby";
    listDesc << "Les programmes de base de QToyunda (GuiliGuili et QToyTime"
              << "Les outils en ligne de commande pour générer les sous titres Toyunda"
              << "Le lecteur toyunda officiel d'epitanime"
              << "Le framework qu'utilise les différents composants de QToyunda"
              << "Sert à executer les principaux Toyunda Tools";

    for (int i = 0; i < listItem.count(); i++)
    {
        item = new QStandardItem();
        listItem[i]->setText(listTitle[i]);
        item->setText(listDesc[i]);
        item->setEditable(false);
        model->setItem(i, 0, listItem[i]);
        model->setItem(i, 1, item);
    }
}

void QToyundaDownloader::downloadFile(QString file, QUrl url)
{
    m_file = new QFile(file);
    m_file->open(QIODevice::WriteOnly);
    qDebug() << file;
    m_abortRequest = false;
    startRequest(url);
}

void QToyundaDownloader::startRequest(QUrl url)
{
    m_reply = m_qnam.get(QNetworkRequest(url));
    connect(m_reply, SIGNAL(finished()),
            this, SLOT(httpFinished()));
    connect(m_reply, SIGNAL(readyRead()),
            this, SLOT(httpReadyRead()));
    connect(m_reply, SIGNAL(downloadProgress(qint64,qint64)),
            this, SLOT(updateDataReadProgress(qint64,qint64)));
}


void QToyundaDownloader::nextDL()
{
    if (m_toDL.isEmpty())
    {
        if (!m_abortRequest)
        {
            if (m_7zipExec.isEmpty())
            {
                QMessageBox::information(this, "7-zip non trouvé", "7-Zip n'a pas pu être trouvé, vous devrez décompressez les fichiers vous même");
                return ;
            }
            QMessageBox::StandardButton b = QMessageBox::question(this, "Decompression des fichiers possibles",
                                                                  "7-Zip est présent, voulez-vous décompressez les fichiers ?",
                                                                  QMessageBox::Ok | QMessageBox::No);
            if (b == QMessageBox::Ok)
            {
                decompressFile();
            }
        }
        return ;
    }
    QString path = m_toDL.first();
    m_toDL.removeFirst();

    QUrl url(m_config->value("baseURL").toString() + "/" + path);
    QFileInfo fi(url.path());
    m_progressDialog->setWindowTitle(fi.fileName());
    m_progressDialog->setLabelText("Donwloading : " + url.path());
    downloadFile(m_dlPath + "/" + fi.fileName(), url);
}

void QToyundaDownloader::nextDeZip()
{
    if (m_todezip.isEmpty())
    {
        m_progressDialog->hide();
        QMessageBox::information(this, "Décompression terminée", "Décompression terminée");
        return ;
    }
    m_7zipProcess = new QProcess();
    connect(m_7zipProcess, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(dezipProcessFinished(int, QProcess::ExitStatus)));
    QStringList args;


    if (QFileInfo(m_todezip.first()).fileName() == m_config->value("QToyunda").toString())
        args << "e";
    else
        args << "x";
    args << "-y" << "-o" + m_dezipPath << QFileInfo(m_todezip.first()).fileName();
    qDebug() << m_7zipExec << args;

    m_progressDialog->setWindowTitle("Decompression des fichiers");
    m_progressDialog->setLabelText("Décompression de : " + QFileInfo(m_todezip.first()).fileName());
    m_progressDialog->show();
    m_todezip.removeFirst();

    m_7zipProcess->start(m_7zipExec, args);
}

void QToyundaDownloader::httpFinished()
{
    if (m_abortRequest)
    {
        m_file->flush();
        m_file->remove();
        delete m_file;
        m_file = 0;
        m_reply->deleteLater();
        m_progressDialog->hide();
        return ;
    }
    if (m_file) {
        m_file->flush();
        m_file->close();
        delete m_file;
        m_file = 0;
    }
    m_reply->deleteLater();
    m_progressDialog->hide();
    nextDL();
    return;
}

void QToyundaDownloader::httpReadyRead()
{
    if (m_file)
        m_file->write(m_reply->readAll());
}

void QToyundaDownloader::updateDataReadProgress(qint64 bytesRead, qint64 totalBytes)
{
    if (!m_abortRequest)
    {
        m_progressDialog->setMaximum(totalBytes);
        m_progressDialog->setValue(bytesRead);
    }
}

void QToyundaDownloader::cancelDownload()
{
    m_abortRequest = true;
    m_reply->abort();
}

void QToyundaDownloader::dezipProcessFinished(int exitCode, QProcess::ExitStatus qpExitStatus)
{
    qDebug() << exitCode;
    if (exitCode == 0)
        nextDeZip();
    else
    {
        nextDeZip();
    }
}

void QToyundaDownloader::on_pushButton_clicked()
{
    close();
}

void QToyundaDownloader::on_dlButton_clicked()
{

    m_toDL.clear();

    if (m_qtoyItem.checkState() == Qt::Checked)
        m_toDL << m_config->value("QToyunda").toString();
    if (m_toyToolItem.checkState() == Qt::Checked)
        m_toDL << m_config->value("Toyunda-Tool").toString();
    if (m_mpItem.checkState() == Qt::Checked)
        m_toDL << m_config->value("Win32").toString() + "/" + m_config->value("MPlayerToyunda").toString();
    if (m_gstItem.checkState() == Qt::Checked)
        m_toDL << m_config->value("Win32").toString() +  "/" + m_config->value("GStreamer-SDK").toString();
    if (m_rubItem.checkState() == Qt::Checked)
        m_toDL << m_config->value("Win32").toString() +  "/" + m_config->value("Ruby").toString();
    if (m_toDL.isEmpty())
    {
        QMessageBox::information(this, "Erreur", "Vous n'avez rien choisit de telecharger");
        return;
    }
    m_todezip = m_toDL;
    nextDL();
}



void QToyundaDownloader::decompressFile()
{
    QString dirPath = QFileDialog::getExistingDirectory(this, "Choississez le répertoire d'installation", "", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (dirPath.isEmpty())
        return;
    m_dezipPath = dirPath;
    nextDeZip();
}
