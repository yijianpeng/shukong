#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    w.setWindowTitle(QStringLiteral("凸轮设计软件-by易健鹏"));
    w.show();

    return a.exec();
}
