#include "curvelinegenerator.h"
#include "rainpainter.h"
#include <QPainter>
#include "math.h"
#include <QPainterPath>
CurveLineGeneratorSettings::CurveLineGeneratorSettings(QObject* parent) : WallpaperGeneratorSettings(setupPrototype(), parent)
{
    // m_patternPainter=new RainPainter(this);
}

StandardItemModel* CurveLineGeneratorSettings::setupPrototype()
{
    StandardItemModel* mainModel=new StandardItemModel("CurveLineGenerator","CurveLineGenerator");
    mainModel->appendSection(setupStandardSection());
    mainModel->appendSection(setupStartValue());
    mainModel->appendSection(setupSpecificSection());
    return mainModel;
}

StandardItemModel* CurveLineGeneratorSettings::setupSpecificSection()
{
    StandardItemModel* model=new StandardItemModel("Curve parameters");


   model->appendRow(new StandardItem(QObject::tr("p1"),150,"Slider.qml",0,QObject::tr("P1"),QObject::tr(""),-2000,2000));
   model->appendRow(new StandardItem(QObject::tr("p2"),100,"Slider.qml",0,QObject::tr("P2"),QObject::tr(""),-2000,2000));
   model->appendRow(new StandardItem(QObject::tr("p3"),500,"Slider.qml",0,QObject::tr("P3"),QObject::tr(""),-2000,2000));
   model->appendRow(new StandardItem(QObject::tr("p4"),500,"Slider.qml",0,QObject::tr("P4"),QObject::tr(""),-2000,2000));
   model->appendRow(new StandardItem(QObject::tr("p5"),500,"Slider.qml",0,QObject::tr("P5"),QObject::tr(""),-2000,2000));
   model->appendRow(new StandardItem(QObject::tr("p6"),500,"Slider.qml",0,QObject::tr("P6"),QObject::tr(""),-2000,2000));
   model->appendRow(new StandardItem(QObject::tr("p7"),-0.04,"Slider.qml",0,QObject::tr("P7"),QObject::tr(""),-5,5));
   model->appendRow(new StandardItem(QObject::tr("p8"),0.85,"Slider.qml",0,QObject::tr("P8"),QObject::tr(""),-5,5));
   model->appendRow(new StandardItem(QObject::tr("p9"),1.6,"Slider.qml",0,QObject::tr("P9"),QObject::tr(""),-5,5));

    return model;

}

StandardItemModel* CurveLineGeneratorSettings::setupStartValue()
{
    StandardItemModel* model=new StandardItemModel("Start value");
    model->appendRow(new StandardItem(QObject::tr("Start value y"),0,"Slider.qml",0,QObject::tr("Start value for y"),QObject::tr("pixels"),-2000,2000));
    model->appendRow(new StandardItem(QObject::tr("End value y"),0,"Slider.qml",0,QObject::tr("End y value"),QObject::tr("pixels"),-2000,2000));
    return model;
}

CurveLineGenerator::CurveLineGenerator(QQuickItem *parent):WallpaperGenerator(parent)
{
    m_settings=new CurveLineGeneratorSettings(this);
    connect(m_settings->activeModel()->sections()[0],&StandardItemModel::dataChanged, this, &WallpaperGenerator::redraw);
    connect(m_settings->activeModel()->sections()[1],&StandardItemModel::dataChanged, this, &CurveLineGenerator::redraw);
    connect(m_settings->activeModel()->sections()[2],&StandardItemModel::dataChanged, this, &CurveLineGenerator::redraw);

}

void CurveLineGenerator::paint(QPainter *painter, double width, double height)
{
    double i=0;
    double x=0;
    double y=m_settings->getActiveData("Start value y").toDouble();
    double endX=width;
    double endY=m_settings->getActiveData("End value y").toDouble();;
    //int maxIt=20;

    QPen pen=painter->pen();
    pen.setColor(m_settings->baseColor());
    pen.setWidthF(m_settings->stroke());
    painter->setBrush(QBrush(m_settings->backgroundColor()));
    painter->drawRect(QRectF(0,0,width, height));
    painter->setBrush(Qt::NoBrush);
    painter->setRenderHint(QPainter::HighQualityAntialiasing);
    painter->setPen(pen);
    endY=function(endX, endY, m_settings->getActiveData("p1").toDouble());
    y=function(x, y, m_settings->getActiveData("p2").toDouble());
    double cX=function(x, y, m_settings->getActiveData("p3").toDouble());
    double cY=function(y, x, m_settings->getActiveData("p4").toDouble());
    double c2Y=function(y, x, m_settings->getActiveData("p5").toDouble());
    double c2X=function(x, y, m_settings->getActiveData("p6").toDouble());
    i+=1;
    double h;
    double s;
    double v;
    m_settings->baseColor().getHsvF(&h,&s,&v);
    while(i<m_settings->iterations()){
        QPainterPath path;
        if(m_settings->colorful()){

            QColor color=QColor::fromHsvF(fmod(h+i*0.1,1.0f),s,v);
            pen.setColor(color);
            painter->setPen(pen);
        }
        path.moveTo(x,y);
        path.cubicTo(cX, cY, c2X, c2Y, endX, endY);
        painter->drawPath(path);
        endY=function(endX, endY, m_settings->getActiveData("p1").toDouble());
        y=function(x, y, m_settings->getActiveData("p2").toDouble());
        cX=function(x, y, m_settings->getActiveData("p3").toDouble());
        cY=function(y, x,m_settings->getActiveData("p4").toDouble());

        c2Y=function(y, x, m_settings->getActiveData("p6").toDouble());
        c2X=function(x, y,m_settings->getActiveData("p5").toDouble());
        i+=1;
    }
}

double CurveLineGenerator::function(double p1, double p2, double p3)
{
    return  m_settings->getActiveData("p7").toDouble()*p1+ m_settings->getActiveData("p8").toDouble()*p2+ m_settings->getActiveData("p9").toDouble()*p3;
}
