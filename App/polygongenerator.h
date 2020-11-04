#ifndef POLYGONGENERATOR_H
#define POLYGONGENERATOR_H
#include "wallpapergenerator.h"

class PolygonGeneratorSettings : public WallpaperGeneratorSettings
{
public:
    PolygonGeneratorSettings(QObject* parent=nullptr);
    StandardItemModel* setupPrototype();
private :
    StandardItemModel* setupSpecificSection();
    StandardItemModel* setupStartValue();


};

class PolygonGenerator: public WallpaperGenerator
{
public:
    PolygonGenerator(QQuickItem* parent=nullptr);
    virtual void paint(QPainter* painter, double width, double height) override;
private:
    QPolygonF paintPolygon(int edges, double centerX, double centerY, double radius, double alphaBegin);
};

class MetaPolygonGenerator : public MetaGenerator{
    const QString name() override{return "Polygon generator";}
    void init() override {if(m_item!=nullptr)return; else m_item=new PolygonGenerator();}
};

#endif // POLYGONGENERATOR_H
