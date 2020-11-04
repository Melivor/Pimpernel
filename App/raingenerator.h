#ifndef RAINGENERATOR_H
#define RAINGENERATOR_H
#include "wallpapergenerator.h"


class RainGeneratorSettings : public WallpaperGeneratorSettings
{
public:
    RainGeneratorSettings(QObject* parent=nullptr);
    StandardItemModel* setupPrototype();
private :
    StandardItemModel* setupSpecificSection();

};

class RainGenerator: public WallpaperGenerator
{
public:
    RainGenerator(QQuickItem* parent=nullptr);
    virtual void paint(QPainter* painter, double width, double height) override;
};

class MetaRainGenerator : public MetaGenerator{
    const QString name() override{return "Rain generator";}
    void init() override {if(m_item!=nullptr)return; else m_item=new RainGenerator();}
};

#endif // RAINGENERATOR_H
