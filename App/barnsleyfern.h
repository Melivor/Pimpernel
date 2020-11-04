#ifndef BARNSLEYFERN_H
#define BARNSLEYFERN_H
#include "patternpainter.h"

class BarnsleyFern : public PatternPainter
{
public:
    BarnsleyFern();
    virtual void paint(QPainter* painter, double width, double height) override;
    QPointF f1(const QPointF& pt){return QPointF(0,0.16*pt.y());}
    QPointF f2(const QPointF& pt){return QPointF(0.85*pt.x()+0.04*pt.y(),-0.04*pt.x()+0.85*pt.y()+1.6);}
    QPointF f3(const QPointF& pt){return QPointF(0.2*pt.x()-0.26*pt.y(),0.23*pt.x()+0.22*pt.y()+1.6);}
    QPointF f4(const QPointF& pt){return QPointF(-0.15*pt.x()+0.28*pt.y(),0.26*pt.x()+0.24*pt.y()+0.44);};
};

#endif // BARNSLEYFERN_H
