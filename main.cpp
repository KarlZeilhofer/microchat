#include "microchat.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MicroChat w;
    w.show();

    return a.exec();
}
