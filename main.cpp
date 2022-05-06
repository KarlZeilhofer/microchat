#include "microchat.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("Microchat");
    a.setOrganizationName("microchat");
    MicroChat w;
    w.show();

    return a.exec();
}
