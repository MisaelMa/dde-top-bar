#include "panel/mainwindow.h"
#include <DApplication>
DWIDGET_USE_NAMESPACE
int main(int argc, char *argv[])
{
    DApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
