#include "rainpainter.h"
#include <QPainter>
#include <cmath>
RainPainter::RainPainter()
{
    m_parameters={2,2,5,0};
    m_maxValues={50,50,500,M_PI*2};
    m_minValues={2,2,0,0};
}


void RainPainter::paint(QPainter *painter, double width, double height)
{

    QPen pen=painter->pen();
    painter->setBrush(QBrush(m_backgroundColor));
    painter->drawRect(QRectF(0,0,width, height));
    painter->setBrush(Qt::NoBrush);
    pen.setColor(m_strokeColor);
    pen.setWidthF(stroke());
    painter->setRenderHint(QPainter::HighQualityAntialiasing);
    painter->setPen(pen);
    QList<QPointF> pointsList;
    double x=0;
    double y=0;
    while(x<width){
        while(y<height){
            y+=m_parameters[1];
            pointsList.push_back(QPointF(x,y));
        }
        y=0;
        x+=m_parameters[0];
    }
    int i=0;
    while(i<m_maxIt&&pointsList.size()>0){
        int acc=std::rand()/((RAND_MAX + 1u)/float(pointsList.size()));
        QPointF pt=pointsList[acc];
        auto&& pt2=QPointF(pt.x()+cos(m_parameters[3])*m_parameters[2], pt.y()+sin(m_parameters[3])*m_parameters[2]);
        pointsList.removeAt(acc);
        painter->drawLine(QLineF(pt, pt2));
        ++i;
    }
}
