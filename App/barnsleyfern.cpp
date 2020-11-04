#include "barnsleyfern.h"
#include <QPainter>
BarnsleyFern::BarnsleyFern()
{
    m_backgroundColor=QColor::fromHsl(0,0,80,255);
    m_strokeColor=QColor::fromHsl(0,0,255,255);
}

void BarnsleyFern::paint(QPainter *painter, double width, double height)
{
    int i=0;
    QPen pen=painter->pen();
    painter->setBrush(QBrush(m_backgroundColor));
    painter->drawRect(QRectF(0,0,width, height));
    painter->setBrush(Qt::NoBrush);
    pen.setColor(m_strokeColor);
    pen.setWidthF(stroke());
    painter->setRenderHint(QPainter::HighQualityAntialiasing);
    painter->setPen(pen);
    QPointF pt(0, 0);
    painter->translate(width/2, 0);
    painter->scale(100,100);

     painter->drawPoint(pt);
    while (i<m_maxIt) {
        ++i;
        int n=rand()%100;
        if(n<=1){
            pt=f1(pt);
        }
        else if(n<=7){
            pt=f2(pt);
        }
        else if(n<=14){
            pt=f3(pt);
        }
        else{
            pt=f4(pt);
        }
        painter->drawPoint(pt);
    }
}
