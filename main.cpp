#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    w.show();
    //w.comRenewer();
    // w.on_baudRateCb_activated(0);
    w.on_box_available_ports_activated(0);

    return a.exec();
}
