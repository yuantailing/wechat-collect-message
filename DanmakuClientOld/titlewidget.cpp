#include "titlewidget.h"
#include <QPen>
#include <QPainter>

TitleWidget::TitleWidget(QWidget *parent):
    QWidget(parent)
{

}

void TitleWidget::paintEvent(QPaintEvent *){
    // 设置画笔
    QPen objPen(Qt::NoBrush,1);
    QPainter Painter(this);
    Painter.setPen(objPen);
    //设置渐变画刷
    QLinearGradient objLinear(rect().topLeft(),rect().bottomLeft());
    //顶部颜色和透明度
    objLinear.setColorAt(0,QColor(50,50,50,240));
    //中间颜色和透明度
    objLinear.setColorAt(0.8,QColor(34,34,34,255));
    //底部颜色和透明度
    objLinear.setColorAt(1,QColor(7,7,7,255));
    QBrush objBrush(objLinear);
    Painter.setBrush(objBrush);
    //画圆角矩形
    //Painter.drawRoundedRect(rect(),5,5);
    Painter.drawRect(rect());
}
