#include <QCoreApplication>
#include <QProcess>
#include "../comons/sqhandlegstpath.h"
#include "process.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    
    sq_set_gstlib_env();
    _execv(QString(qApp->applicationDirPath() + "/QToyTime.exe").toLatin1().data(), argv);
    return 0;
}
