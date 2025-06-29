#include "mainwindow.h"
#include <QApplication>

using namespace std;

int main(int argc, char *argv[])
{
    // 点击输入文本
    QApplication a(argc, argv);
    MainWindow w;
    //w.show();
    return a.exec();
}
