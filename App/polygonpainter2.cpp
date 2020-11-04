#include "polygonpainter2.h"
#include <cmath>
#include <QPainter>

PolygonPainter2::PolygonPainter2()
{
    m_parameters={6,50,0,1};
    m_minValues={2,1,0,0};
    m_maxValues={50,200,2*M_PI,255};
    m_startValues={0,0};
    m_startValuesMin={0,0};
    m_startValuesMax={1920,1080};
    m_backgroundColor=QColor::fromHsl(0,0,80,255);
    m_strokeColor=QColor::fromHsl(0,0,255,255);
}


void PolygonPainter2::paint(QPainter *painter, double width, double height)
{
    QPen pen=painter->pen();
    painter->setBrush(QBrush(m_backgroundColor));
    painter->drawRect(QRectF(0,0,width, height));
    painter->setBrush(Qt::NoBrush);
    pen.setColor(m_strokeColor);
    pen.setWidthF(stroke());
    painter->setRenderHint(QPainter::HighQualityAntialiasing);
    painter->setPen(pen);
    QPointF center(m_startValues.at(0),m_startValues.at(1));
    double radius=m_parameters[1];
    double startAngle=m_parameters[2];
    double edges=m_parameters[0];
    int opacity=m_parameters[3];
    QPolygonF hexa=paintPolygon(edges,center.x(),center.y(),radius,startAngle);

    painter->drawPolygon(hexa);
    QList<QPointF> nextSpot;
    QList<QPointF> occupiedSpot;
    for(int i=0; i<hexa.size()-1; ++i){
        auto&& pt=hexa[i]+(hexa[i+1]-hexa[i])/2;
        nextSpot.push_back(pt);
    }
    nextSpot.push_back(hexa.back()+(hexa.front()-hexa.back())/2);

    int i=0;
    while(i<m_maxIt){
        if(nextSpot.isEmpty()){
            break;
        }
        ++i;
        int j=rand()%(nextSpot.size());

        center=nextSpot[j];
        nextSpot.removeAt(j);
        occupiedSpot.push_back(center);
        QPolygonF hexa=paintPolygon(edges,center.x(),center.y(),radius,startAngle);
        if(i%2==0){
            painter->setBrush(QColor::fromHsl(m_strokeColor.hslHue(), m_strokeColor.hslSaturation(),m_strokeColor.lightness(),opacity));

        }
        else{
            painter->setBrush(Qt::NoBrush);
        }
        painter->drawPolygon(hexa);
        for(int i=0; i<hexa.size()-1; ++i){
            auto&& pt=hexa[i]+(hexa[i+1]-hexa[i])/2;
            // auto&& npt=center+(pt-center)*2;
            if(pt.x()>0 && pt.x()<width && pt.y()>0 && pt.y()<height && !nextSpot.contains(pt)&&!occupiedSpot.contains(pt)){
                nextSpot.push_back(pt);
            }
        }


        auto lastPt=hexa.back()+(hexa.front()-hexa.back())/2;
        if(lastPt.x()>0 && lastPt.x()<width && lastPt.y()>0 && lastPt.y()<height &&  !nextSpot.contains(lastPt)&&!occupiedSpot.contains(lastPt)){
            nextSpot.push_back(lastPt);
        }

    }
}
QPolygonF PolygonPainter2::paintPolygon(int edges, double centerX, double centerY, double radius, double alphaBegin)
{
    // double alphaBegin=std::acos(xStart);
    double dAlpha=M_PI*2/edges;
    double alpha=alphaBegin+dAlpha;

    QPolygonF polygon;

    while(alpha<=alphaBegin+2*M_PI){
        QPointF pt={centerX+cos(alpha)*radius, centerY+sin(alpha)*radius};
        alpha+=dAlpha;

        polygon.push_back(pt);
    }
    return polygon;
}
