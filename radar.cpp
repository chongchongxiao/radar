#include "radar.h"
#include "QPoint"
#include "QTimer"

Radar::Radar(QWidget *parent)
    : QWidget(parent)
{
    init();
}

Radar::~Radar()
{
}

void Radar::init()
{
    scan_angle=0;
    scan_angle_pre=5; //默认每次旋转15度
    scan_time_pre=33;  //默认每100ms旋转一次
    scan_span_angle=30; //默认扫描弧宽度是30度
    m_cylindernum = 5;  //默认5圈
    m_lineangle = 30; //默认30度
    m_showAxisLabel = true;
    m_showLabel=false;
    isScan=false;
    isFindPoint=false;
    isStopScan=false;
    m_Axis_min = 0;
    m_Axis_max = 100;
    m_AxisLabelList << "0" << "20" << "40" << "60" << "80" << "100";

    for (int i = 0 ; i < 360; i+=m_lineangle)
    {
        m_labelList << QString("%1").arg(i);
    }

}


void Radar::paintEvent( QPaintEvent *e )
{
    drawInit();
    drawArc();
    drawPoint();
}

void Radar::drawInit()
{
    QPainter *painter = new QPainter(this);
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setRenderHint(QPainter::SmoothPixmapTransform);
    painter->setRenderHint(QPainter::TextAntialiasing);

    //添加背景颜色
    painter->save();
    painter->setBrush(Qt::black);
    painter->drawRect(0,0,width(),height());
    painter->restore();
//////////////////////////////////////////////////////////////////////////

    drawRadar(painter);
    drawLabel(painter);
    //drawLinePoints(painter);
    drawAxisLabel(painter);
    //drawArc(painter,0*16);
    delete(painter);
}

void Radar::drawRadar( QPainter *painter )
{
    painter->save();
    QPen pen;
    QColor color(Qt::green);
    pen.setWidthF(0.8);
    pen.setColor(color/*.lighter(100)*/); //添加线的颜色
    painter->setPen(pen);

    QPainterPath looppath;
    int w = width();
    int h = height();

    int count = m_cylindernum;
    int radius = qMin(w,h);
    int step = radius/(count+1);  //加1是为了四周留出空间，写标签
    int x = w*0.2;
    int y = h*0.2;

    QRectF outrect;
    //画雷达的圆圈
    for (int i = 1; i < count+1; ++i)
    {
        radius = step*i;
        x = w/2-radius/2;
        y = h/2-radius/2;
        QRectF rect(x,y,radius,radius);
        looppath.addEllipse(rect);
        outrect = rect;
    }
    //qDebug()<<"w   h"<<w<<h<<endl;
    //qDebug()<<x<<" "<<y<<" "<<radius<<endl;
    painter->drawPath(looppath);
//////////////////////////////////////////////////////////////////////////

    //画雷达的线
    if (m_lineangle == 0)
    {
        m_lineangle =90;
    }
    int linecount = 360/m_lineangle;
    int x0 = w/2 ;
    int y0 = h/2;
    int newradius = outrect.height()/2;
    for (int i = 0 ; i < linecount; ++i)
    {
        int x1 = x0 + newradius*qSin(PI*2/linecount*i);
        int y1 = y0 + newradius*qCos(PI*2/linecount*i);

        painter->drawLine(x0,y0,x1,y1);
    }

    painter->restore();

}

void Radar::drawAxisLabel( QPainter *painter )
{
    if (!m_showAxisLabel) return;

    painter->save();

    QPen pen;
    QColor color(Qt::green);
    pen.setColor(color/*.lighter(100)*/); //添加线的颜色
    painter->setPen(pen);


    int  laycount = m_cylindernum;  //默认是几层
    int w = width();
    int h = height();
    //int count = 360/m_lineangle;
    int radius = qMin(w,h)/2;
    int x0 = w/2 ;
    int y0 = h/2;
    QFont font;
    int fontsize = 10;
    if (h > 500)
        fontsize = 15;
    font.setPointSize(fontsize);
    painter->setFont(font);

    for (int j = 0 ; j < laycount+1; ++j)
    {
        int newradius = radius*j/(laycount+1);
        int x1 = x0 + newradius*qSin(PI/2);
        int y1 = y0 + newradius*qCos(PI/2);
        if (j < m_AxisLabelList.count())
        {
            painter->drawText(x1,y1,m_AxisLabelList.at(j));
        }
    }
    painter->restore();
}

void Radar::drawLabel( QPainter *painter )
{
    if (!m_showLabel) return;

    painter->save();

    QPen pen;
    QColor color(Qt::green);
    pen.setColor(color/*.lighter(100)*/); //添加线的颜色
    painter->setPen(pen);


    int  laycount = m_cylindernum;  //默认是几层
    int w = width();
    int h = height();
    int count = 360/m_lineangle;
    int radius = qMin(w,h)/2;
    int x0 = w/2 ;
    int y0 = h/2;

    QFont font;
    int fontsize = 10;
    if (h > 500)
        fontsize = 15;
    font.setPointSize(fontsize);
    painter->setFont(font);


    for (int i = 0 ; i < count; ++i)
    {
        int newradius = radius*laycount/(laycount+1)+10;
        newradius = newradius+10*qSin(PI*2/count*i);
        int x1 = x0 - newradius*qSin(PI*2/count*i);
        int y1 = y0 - newradius*qCos(PI*2/count*i);

        if (y0 < y1 && x0 < x1)
        {
            y1 -= fontsize*qSin(PI*2/count*i);
        }
        else
        {
            if (x1 < x0)
                x1-=fontsize;
            y1 += fontsize*qSin(PI*2/count*i);
        }

        if (i < m_labelList.count())
        {
            painter->drawText(x1,y1,m_labelList.at(i));
        }
    }
    painter->restore();
}

void Radar::drawArc()
{
    if(!isScan) return;
    //qDebug()<<startAngle<<endl;
    //double scan=scan_angle/360.0;
    QPainter *painter=new QPainter(this);
    painter->save();
    int w = width();
    int h = height();
    int count = m_cylindernum;
    int radius = qMin(w,h);//表示整个圆的直径
    int draw_radius = (radius*count)/(count+1);//表示要画的圆的最大直径
    //qDebug()<<"ww   hh"<<w<<h<<endl;
    //qDebug()<<radius<<" "<<draw_radius<<endl;
    //圆心坐标
    int x0 = w/2;
    int y0 = h/2;
    //最大圆的左上角坐标
    int x = w/2-draw_radius/2;
    int y = h/2-draw_radius/2;
    QConicalGradient conicalGradient(QPointF(x0,y0),-scan_angle);
    conicalGradient.setColorAt(0, Qt::green);
    conicalGradient.setColorAt(1.0/12, Qt::black);
    conicalGradient.setColorAt(1.0/12, QColor(0x00,0xff,0x00,0x00));
    conicalGradient.setColorAt(1, QColor(0x00,0xff,0x00,0x00));
    painter->setBrush(conicalGradient);
    //qDebug()<<x<<" "<<y<<endl;
    QRectF rect_top(x, y, draw_radius, draw_radius);//设置圆形的矩形轮廓
    painter->setPen(Qt::green);
    painter->drawEllipse(rect_top);

    painter->restore();
    delete(painter);
}

void Radar::drawPoint()
{
    if(!isFindPoint) return;
    QPainter *painter=new QPainter(this);
    int w = width();
    int h = height();
    int count = m_cylindernum;
    int radius = qMin(w,h);//表示整个圆的直径
    int draw_radius = (radius*count)/(count+1);//表示要画的圆的最大直径
    //qDebug()<<"ww   hh"<<w<<h<<endl;
    //qDebug()<<radius<<" "<<draw_radius<<endl;
    //圆心坐标
    int x0 = w/2;
    int y0 = h/2;

    QPointF point(x0+draw_radius/4, y0+draw_radius/4);//画点的位置
    double pointRadius=3.5; //画点的半径
    //1. 开始需要指定圆心center(x0, y0)与半径queal(r)，这样就确定了一个圆，然后再指定一个焦点focalPoint(x1, y1)。
    //   焦点的位置为0， 圆环的位置为1，然后在焦点和圆环间插入颜色。
    //   这两个点就是为了指定，颜色渐变的方向
    QRadialGradient radialGradient(point,pointRadius, point);
    //2. 插入颜色
    radialGradient.setColorAt(0, Qt::red);
    radialGradient.setColorAt(1, Qt::red);
    //3. 设置区域外的扩散方式
    radialGradient.setSpread(QGradient::RepeatSpread);
    //4. 使用渐变作为画刷
    painter->setBrush(radialGradient);
    //5. 开画
    painter->drawEllipse(point, pointRadius, pointRadius);
}

bool Radar::findPoint()
{
    return true;
}

void Radar::drawScan()
{
    if(!isScan) return;
    while(scan_angle>=360&&scan_angle>0)
    {
        scan_angle-=360;
    }
    this->repaint();
    scan_angle+=scan_angle_pre;
}

void Radar::startScan()
{
    if(isScan&&!isStopScan) return;//只有非扫描状态或者扫描且停止状态才继续start
    isScan=true;
    isFindPoint=findPoint();
    if(timer==NULL)
    {
        timer = new QTimer(this);
        //timer->setSingleShot(true)
        QObject::connect(timer, SIGNAL(timeout()), this, SLOT(drawScan()));
          //时间间隔
    }
    if(!timer->isActive())
    {
        timer->start(scan_time_pre);
    }
}

void Radar::stopScan()
{
    if(timer==NULL) return;
    if(timer->isActive())
    {
        timer->stop();
        isStopScan=true;
    }
}

void Radar::end()
{
    scan_angle=0;//初始化扫描角度
    isScan=false;//扫描状态置为否
    isFindPoint=false;//初始化找到点的状态
    isStopScan=false;//初始化是否停止扫描状态
    delete timer;
    timer=NULL;
    this->repaint();
}
