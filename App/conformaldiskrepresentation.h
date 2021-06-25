#ifndef CONFORMALDISKREPRESENTATION_H
#define CONFORMALDISKREPRESENTATION_H
#include "wallpapergenerator.h"
#include "math.h"
#include "ConformalDiskModel.h"


class ConformalDiskRepresentationSettings : public WallpaperGeneratorSettings
{
public:
    ConformalDiskRepresentationSettings(QObject* parent=nullptr);
    StandardItemModel* setupPrototype();
    void setDefaultFunctionParameters();
private :

    StandardItemModel* setupSpecificSection();
    StandardItemModel* setupStartValue();
    StandardItemModel* setupStandardSection();
    StandardItemModel* setupDiskParameters();


};

class ConformalDiskRepresentation: public WallpaperGenerator
{
public:
    ConformalDiskRepresentation(QQuickItem* parent=nullptr);
    virtual void paint(QPainter* painter, double width, double height) override;
    void squareTilling(QPainter* painter, const QPointF& center, double diameter);
    void drawSquareInConformalDisk(QPainter* painter, const QPointF& center, double diameter, double squareWidth, const QPointF &squareCenter);
private:
    void unitArcToArc(const QPointF &center, double diameter, QPointF &arcCenter, double& arcRadius, QPointF &start, QPointF &end);
    void drawArc(QPainter* painter, const QPointF &arcCenter, double arcRadius, const QPointF &start, const QPointF &end, bool dir=1);

};

class MetaConformalDiskRepresentation : public MetaGenerator{
    const QString name() override{return "Conformal disk representation";}
    void init() override {if(m_item!=nullptr)return; else m_item=new ConformalDiskRepresentation();}
};

#endif // CONFORMALDISKREPRESENTATION_H
