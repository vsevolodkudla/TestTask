#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QThread thread;
    MainWindow w;
    w.show();

    return a.exec();
}
