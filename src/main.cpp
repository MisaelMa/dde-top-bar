#include "panel/mainwindow.h"
#include <DApplication>

DWIDGET_USE_NAMESPACE
int main(int argc, char *argv[])
{


    DApplication::loadDXcbPlugin();
    DApplication app(argc, argv);
    app.setTheme("light");
    app.setSingleInstance("deepin-topbar");
    app.setApplicationName("deepin-topbar");
    app.setOrganizationName("deepin");
    MainPanel panel;
    panel.show();
    return app.exec();

}
