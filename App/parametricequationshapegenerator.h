#ifndef PARAMETRICEQUATIONSHAPEGENERATOR_H
#define PARAMETRICEQUATIONSHAPEGENERATOR_H
#include "wallpapergenerator.h"
#include "parametricequation.h"
#include "parametricequationanimator.h"
class ParametricEquationAnimator;
class ParametricEquationsShapeGeneratorSettings : public WallpaperGeneratorSettings
{
    Q_OBJECT
public:
    ParametricEquationsShapeGeneratorSettings(QObject* parent=nullptr);
    StandardItemModel* setupPrototype();
    QPointF pt(double t);
    QString xEquation(){return "x="+m_xEquation.expression();}
    QString yEquation(){return "y="+m_yEquation.expression();}
    Q_INVOKABLE QAbstractItemModel* getAnimator(){return m_animator.get();}
private :
    void resetAnimator();
    StandardItemModel* setupSpecificSection();
    StandardItemModel* setupStartValue();
    StandardItemModel* setupStandardSection();
    StandardItemModel* setupFunctionParameters();
    StandardItemModel* setupAnimationParameters();
    std::unique_ptr<ParametricEquationAnimator> m_animator;
    ParametricEquation m_xEquation;
    ParametricEquation m_yEquation;


};

class ParametricEquationsShapeGenerator: public WallpaperGenerator
{
public:
    ParametricEquationsShapeGenerator(QQuickItem* parent=nullptr);
    virtual void paint(QPainter* painter, double width, double height) override;
private:
    void drawLegend(QPainter *painter, double width);
};

class MetaParametricEquationsShapeGenerator : public MetaGenerator{
    const QString name() override{return "Parametric equations shape generator";}
    void init() override {if(m_item!=nullptr)return; else m_item=new ParametricEquationsShapeGenerator();}
};

#endif // PARAMETRICEQUATIONSHAPEGENERATOR_H
