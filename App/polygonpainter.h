#ifndef POLYGONPAINTER_H
#define POLYGONPAINTER_H
#include "patternpainter.h"

class PolygonPainter : public PatternPainter
{
public:
    PolygonPainter();
    virtual void paint(QPainter* painter, double width, double height) override;
    QPolygonF paintPolygon(int edges, double centerX, double centerY, double radius, double alphaBegin);
    void setStartParameters();
};

class MetaPolygonPainter : public MetaPainter{
    const QString name() override{return "Polygons";}
    void init() override {if(m_item!=nullptr)return; else m_item=new PolygonPainter();}
};

#endif // POLYGONPAINTER_H
