#ifndef RADAR_H
#define RADAR_H

#include <QWidget>
#include <QPainter>
#include <QPainterPath>
#include <QMap>
#include <QtDebug>
#include "qmath.h"

#define  PI 3.14159

class Radar : public QWidget
{
    Q_OBJECT

public:
    Radar(QWidget *parent = 0);
    ~Radar();
    void startScan();//扫描外部接口
    void stopScan();
    void end();

protected:
    void paintEvent(QPaintEvent *e);

private:
    void init();
    void drawInit();//初始化雷达
    void drawRadar(QPainter *painter); //画雷达圆圈和线
    void drawAxisLabel(QPainter *painter);//添加标签刻度
    void drawLabel(QPainter *painter);  //画雷达图的角度刻度
    void drawArc();   //画扫描的弧度
    void drawPoint();
    bool findPoint();
    //void drawData(QPainter *painter);
    //void drawScatterPoints(QPainter *painter);
    //void drawLinePoints(QPainter *painter);

private slots:
    void drawScan();//实现图像的扫描效果

private:
    int scan_angle;    //扫描现在的角度
    double scan_angle_pre; //扫描每次旋转的角度
    int scan_time_pre;  //扫描速率，这里是经过多少ms旋转一次
    int scan_span_angle; //扫描扇形的宽度
    int m_cylindernum;  //圈数
    int m_lineangle;    //角度，决定多少。默认是30
    int m_Axis_max,m_Axis_min; //最大最小
    QStringList m_AxisLabelList; //标签 刻度标签
    QStringList m_labelList;     //外围标签
    bool m_showAxisLabel;
    bool m_showLabel;
    bool isScan; //是否扫描的标志
    bool isFindPoint;//是否找到点
    bool isStopScan;
    QTimer *timer;
};
#endif // RADAR_H
