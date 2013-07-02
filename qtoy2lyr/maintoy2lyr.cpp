

#include <QApplication>
#include "qtoy2lyr.h"
#include "sqhandlegstpath.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QToy2lyr w;

    sq_set_gstlib_env();
    w.show();

    return a.exec();
}
