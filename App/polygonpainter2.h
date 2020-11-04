    #ifndef POLYGONPAINTER2_H
#define POLYGONPAINTER2_H
#include "patternpainter.h"

class PolygonPainter2: public PatternPainter
{

    Q_OBJECT
    QML_ELEMENT
public:
    PolygonPainter2();
    virtual void paint(QPainter* painter, double width, double height) override;
    QPolygonF paintPolygon(int edges, double centerX, double centerY, double radius, double alphaBegin);
};
class MetaPolygonPainter2 : public MetaPainter{
    const QString name() override{return "Polygons 2";}
    void init() override {if(m_item!=nullptr)return; else m_item=new PolygonPainter2();}
};


#endif // POLYGONPAINTER2_H
