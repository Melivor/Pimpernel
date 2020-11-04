#include "raingenerator.h"
#include "rainpainter.h"
#include <QPainter>
#include "math.h"
RainGeneratorSettings::RainGeneratorSettings(QObject* parent) : WallpaperGeneratorSettings(setupPrototype(), parent)
{
    // m_patternPainter=new RainPainter(this);
}

StandardItemModel* RainGeneratorSettings::setupPrototype()
{
    StandardItemModel* mainModel=new StandardItemModel("RainGenerator","RainGenerator");
    mainModel->appendSection(setupStandardSection());
    mainModel->appendSection(setupSpecificSection());
    return mainModel;
}

StandardItemModel* RainGeneratorSettings::setupSpecificSection()
{
    StandardItemModel* model=new StandardItemModel("Specific");

    model->appendRow(new StandardItem(QObject::tr("Angle"),90,"Slider.qml",0,QObject::tr("Angle of rain"),QObject::tr("degrees"),0,180));
    model->appendRow(new StandardItem(QObject::tr("x spacing"),20,"Slider.qml",0,QObject::tr("x spacing"),QObject::tr("pixels"),0.1,500));
    model->appendRow(new StandardItem(QObject::tr("y spacing"),20,"Slider.qml",0,QObject::tr("y spacing"),QObject::tr("pixels"),0.1,500));
    model->appendRow(new StandardItem(QObject::tr("Rain drop length"),5,"Slider.qml",0,QObject::tr("Rain drop length"),QObject::tr("pixels"),0.1,500));

    return model;
}

RainGenerator::RainGenerator(QQuickItem *parent):WallpaperGenerator(parent)
{
    m_settings=new RainGeneratorSettings(this);
    connect(m_settings->activeModel()->sections()[0],&StandardItemModel::dataChanged, this, &WallpaperGenerator::redraw);
    connect(m_settings->activeModel()->sections()[1],&StandardItemModel::dataChanged, this, &RainGenerator::redraw);

}

void RainGenerator::paint(QPainter *painter, double width, double height)
{

    QPen pen=painter->pen();
    painter->setBrush(QBrush(m_settings->backgroundColor()));
    painter->drawRect(QRectF(0,0,width, height));
    painter->setBrush(Qt::NoBrush);
    double h;
    double s;
    double v;
    m_settings->baseColor().getHsvF(&h,&s,&v);

    pen.setColor(m_settings->baseColor());
    pen.setWidthF(m_settings->stroke());
    pen.setCapStyle(Qt::RoundCap);
    painter->setRenderHint(QPainter::HighQualityAntialiasing);
    painter->setPen(pen);
    QList<QPointF> pointsList;
    double x=0;
    double y=0;

    while(x<width){
        while(y<height){

            pointsList.push_back(QPointF(x,y));
            y+=m_settings->getActiveData("y spacing").toDouble();
        }
        y=0;
        x+=m_settings->getActiveData("x spacing").toDouble();
    }
    int i=0;
    while(i<m_settings->iterations()&&pointsList.size()>0){
        if(m_settings->colorful()){
            QColor color=QColor::fromHsvF(fmod(h+i*0.1,1.0f),s,v);
            pen.setColor(color);
            painter->setPen(pen);
        }

        int acc=std::rand()/((RAND_MAX + 1u)/float(pointsList.size()));
        QPointF pt=pointsList[acc];
        auto&& pt2=QPointF(pt.x()+cos(M_PI/180*m_settings->getActiveData("Angle").toDouble())*m_settings->getActiveData("Rain drop length").toDouble(), pt.y()+sin(M_PI/180*m_settings->getActiveData("Angle").toDouble())*m_settings->getActiveData("Rain drop length").toDouble());
        pointsList.removeAt(acc);
        painter->drawLine(QLineF(pt, pt2));
        ++i;
    }
}
