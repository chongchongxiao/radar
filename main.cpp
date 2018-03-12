/*
*创建人    李冲
*创建时间  2017年9月
*功能      绘制雷达扫描图
*/

#include "mainwindow.h"
#include "radar.h"
#include "test.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setMinimumSize(400,400);
    //w.setBaseSize(800,600); 设置窗口初始大小，但是无效
    w.resize(800,600);
    //w.start();
    w.show();
    //w.start();
    //Test t;
    //t.show();
    return a.exec();
}
