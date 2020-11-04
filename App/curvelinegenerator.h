#ifndef CURVELINEGENERATOR_H
#define CURVELINEGENERATOR_H
#include "wallpapergenerator.h"

class CurveLineGeneratorSettings : public WallpaperGeneratorSettings
{
public:
    CurveLineGeneratorSettings(QObject* parent=nullptr);
    StandardItemModel* setupPrototype();
    //virtual void paint(QPainter* painter) override;
private :
    StandardItemModel* setupSpecificSection();
    StandardItemModel* setupStartValue();

};

class CurveLineGenerator: public WallpaperGenerator
{
public:
    CurveLineGenerator(QQuickItem* parent=nullptr);
    virtual void paint(QPainter* painter, double width, double height) override;
private:
    double function(double p1, double p2, double p3);
};

class MetaCurveLineGenerator : public MetaGenerator{
    const QString name() override{return "Curve line generator";}
    void init() override {if(m_item!=nullptr)return; else m_item=new CurveLineGenerator();}
};


#endif // CURVELINEGENERATOR_H
