#include <QApplication>
#include "qtoy2lyr.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QToy2lyr w;
    w.show();
    
    return a.exec();
}
