#include "toyundagendialog.h"
#include "ui_toyundagendialog.h"
#include <QDebug>

ToyundaGenDialog::ToyundaGenDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ToyundaGenDialog)
{
    ui->setupUi(this);
    m_process = new QProcess;
    connect(m_process, SIGNAL(readyReadStandardError()), this, SLOT(onReadyStdError()));
}

ToyundaGenDialog::~ToyundaGenDialog()
{
    delete ui;
}

void ToyundaGenDialog::execute(QString exec, QStringList args, QString fileoutput, QString wd)
{
    ui->executeLabel->setText(exec + " " + args.join(" ") + " > " + fileoutput);
    ui->textEdit->clear();
    m_process->setWorkingDirectory(wd);
    m_process->setStandardOutputFile(fileoutput);
    m_process->start(exec, args);
    this->exec();
}

void ToyundaGenDialog::onReadyStdError()
{
    ui->textEdit->insertPlainText(m_process->readAllStandardError());
}

void ToyundaGenDialog::on_closeButton_clicked()
{
    m_process->kill();
    accept();
}
