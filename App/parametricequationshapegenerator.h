#ifndef PARAMETRICEQUATIONSHAPEGENERATOR_H
#define PARAMETRICEQUATIONSHAPEGENERATOR_H
#include "wallpapergenerator.h"
#include "math.h"


struct ParametricFunction{
    QString equationString;
    QString equationXStringTemplate;
    QString equationYStringTemplate;
    std::function<double(double, double, double)> equationX;
    std::function<double(double, double, double)> equationY;
    QString equationXString(double P1,double P2){QString result=equationXStringTemplate; return result.replace("P1",QString::number(P1)).replace("P2",QString::number(P2));}
    QString equationYString(double P1,double P2){QString result=equationYStringTemplate;return result.replace("P1",QString::number(P1)).replace("P2",QString::number(P2));}
    double tmin;
    double tmax;
    double p1;
    double p2;
};

static ParametricFunction parametricFunction1={"Equation 1",
                                               "x=10sin(P1t)round(sqrt(cos(cos(P2t)))",
                                               "y=9sqrt(cos(P1t))sin(sin(P2t))",
                                               [](double t, double p1, double p2){return 10*sin(p1*t)*round(sqrt(cos(cos(p2*t))));},
                                               [](double t, double p1, double p2){return 9*pow(cos(p1*t),2)*sin(sin(p2*t));},
                                               -6.2,
                                               6.2,
                                               2.78,
                                               8.2
                                              };
static ParametricFunction parametricFunction2={"Equation 2",
                                               "x=7*sin(P1*t)/(1+pow(cos(P2*t),2))",
                                               "y=7*cos(P2*t)*pow(sin(P1*t),4)",
                                               [](double t,double p1, double p2){return 7*sin(p1*t)/(1+pow(cos(p2*t),2));},
                                               [](double t,double p1, double p2){return 7*cos(p2*t)*pow(sin(p1*t),4);},
                                               -3,
                                               3,
                                               7.32,
                                               1.42
                                              };
static ParametricFunction parametricFunction3={"Equation 3",
                                               "x=7*sin(p2*t)",
                                               "y=7*cos(p1*t)",
                                               [](double t,double p1, double p2){return 7*sin(p2*t);},
                                               [](double t,double p1, double p2){return  7*cos(p1*t);},
                                               -6,
                                               6,
                                               9.9,
                                               10
                                              };
static ParametricFunction parametricFunction4={"Equation 4",
                                               "x=10*sin(P1*t)*(cos(cos(P2*t))",
                                               "y=9*pow(cos(P1*t),2)*sin(P2*t)",
                                               [](double t,double p1, double p2){return 10*sin(p1*t)*(cos(cos(p2*t)));},
                                               [](double t,double p1, double p2){return 9*pow(cos(p1*t),2)*sin(p2*t);},
                                               -6,
                                               6,
                                               9.9,
                                               10
                                              };
static ParametricFunction parametricFunction5={"Equation 5",
                                               "x=exp(sin(P1*t))*((cos(P2*t)))",
                                               "y=exp(cos(P1*t))*sin(P2*t)",
                                               [](double t,double p1, double p2){return 5*exp(sin(p1*t))*((cos(p2*t)));},
                                               [](double t,double p1, double p2){return 5*exp(cos(p1*t))*sin(p2*t);},
                                               -6,
                                               6,
                                               9.9,
                                               10
                                              };
static ParametricFunction parametricFunction6={"Equation 6",
                                               "x=4*exp(sin(P2*t))*((sin(P1*t)))",
                                               "y=4*exp(cos(P1*t))*cos(P2*t)",
                                               [](double t,double p1, double p2){return 4*exp(sin(p2*t))*((sin(p1*t)));},
                                               [](double t,double p1, double p2){return 4*exp(cos(p1*t))*cos(p2*t);},
                                               -6,
                                               6,
                                               9.9,
                                               10
                                              };


static QList<ParametricFunction> parametricFunctions={parametricFunction1, parametricFunction2, parametricFunction3, parametricFunction4, parametricFunction5, parametricFunction6};

inline QVariantList getEquationsName(){
    QVariantList result;
    for(auto function:parametricFunctions){
        result.push_back(function.equationString);
    }
    return result;
}
class ParametricEquationsShapeGeneratorSettings : public WallpaperGeneratorSettings
{
public:
    ParametricEquationsShapeGeneratorSettings(QObject* parent=nullptr);
    StandardItemModel* setupPrototype();
    void setDefaultFunctionParameters();
private :

    StandardItemModel* setupSpecificSection();
    StandardItemModel* setupStartValue();
    StandardItemModel* setupStandardSection();
    StandardItemModel* setupFunctionParameters();


};

class ParametricEquationsShapeGenerator: public WallpaperGenerator
{
public:
    ParametricEquationsShapeGenerator(QQuickItem* parent=nullptr);
    virtual void paint(QPainter* painter, double width, double height) override;

private:
    void populateEquations();
    std::map<QString,ParametricFunction> m_parametricFunctions;
    std::map<QString, std::function<double(double)>> m_computeXFunctions;
    std::map<QString, std::function<double(double)>> m_computeYFunctions;
    std::function<double(double)> computeX;
    std::function<double(double)> computeY;
};

class MetaParametricEquationsShapeGenerator : public MetaGenerator{
    const QString name() override{return "Parametric equations shape generator";}
    void init() override {if(m_item!=nullptr)return; else m_item=new ParametricEquationsShapeGenerator();}
};

#endif // PARAMETRICEQUATIONSHAPEGENERATOR_H
