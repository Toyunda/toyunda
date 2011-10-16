#include <QtGui/QApplication>
#include "GuiliGuili.h"



int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    GuiliGuili foo;
    foo.show();
    return app.exec();
}
