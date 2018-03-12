#include "test.h"
#include "QPainter"
#include "QImage"
#include "QLinearGradient"
#include "QPointF"
#include "QColor"
Test::Test(QWidget *parent) : QWidget(parent)
{

}

void Test::paintEvent( QPaintEvent *e )
{
    QPainter *painter=new QPainter(this);
    //1. 开始需要指定圆心center(x0, y0)与半径queal(r)，这样就确定了一个圆，然后再指定一个焦点focalPoint(x1, y1)。
    //   焦点的位置为0， 圆环的位置为1，然后在焦点和圆环间插入颜色。
    //   这两个点就是为了指定，颜色渐变的方向
    QRadialGradient radialGradient(QPointF(200, 190), 50, QPointF(200, 190));

    //2. 插入颜色
    radialGradient.setColorAt(0, Qt::green);
    radialGradient.setColorAt(1, Qt::black);

    //3. 设置区域外的扩散方式
    radialGradient.setSpread(QGradient::RepeatSpread);

    //4. 使用渐变作为画刷
    painter->setBrush(radialGradient);

    //5. 开画
    painter->drawEllipse(QPointF(200, 190), 50, 50);
}
