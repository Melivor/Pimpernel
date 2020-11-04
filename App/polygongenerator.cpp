#include "polygongenerator.h"
#include <QPainter>
#include "math.h"
PolygonGeneratorSettings::PolygonGeneratorSettings(QObject* parent) : WallpaperGeneratorSettings(setupPrototype(), parent)
{
    // m_patternPainter=new RainPainter(this);
}

StandardItemModel* PolygonGeneratorSettings::setupPrototype()
{
    StandardItemModel* mainModel=new StandardItemModel("PolygonGenerator","PolygonGenerator");
    mainModel->appendSection(setupStandardSection());
    mainModel->appendSection(setupStartValue());
    mainModel->appendSection(setupSpecificSection());
    return mainModel;
}

StandardItemModel* PolygonGeneratorSettings::setupSpecificSection()
{

    StandardItemModel* model=new StandardItemModel("Specific");

    model->appendRow(new StandardItem(QObject::tr("Edge number"),20,"SliderInt.qml",0,QObject::tr("Polygon number of edges"),QObject::tr(""),0,10));
    model->appendRow(new StandardItem(QObject::tr("Radius"),50,"Slider.qml",0,QObject::tr("Polygon radius"),QObject::tr("pixels"),1,200));
    model->appendRow(new StandardItem(QObject::tr("Angle"),0,"Slider.qml",0,QObject::tr("Angle of polygon"),QObject::tr("degrees"),0,2*M_PI));
    model->appendRow(new StandardItem(QObject::tr("Opacity"),5,"SliderInt.qml",0,QObject::tr("Opacity of polygon"),QObject::tr(""),0,255));
    model->appendRow(new StandardItem(QObject::tr("Spacing"),5,"Slider.qml",0,QObject::tr("Space between polygons"),QObject::tr("pixels"),0.1,20));

    return model;
}

StandardItemModel* PolygonGeneratorSettings::setupStartValue()
{
    StandardItemModel* model=new StandardItemModel("Start value");
    model->appendRow(new StandardItem(QObject::tr("Start position y"),0,"Slider.qml",0,QObject::tr("Start position y"),QObject::tr(""),0,2000));
    model->appendRow(new StandardItem(QObject::tr("Start position x"),0,"Slider.qml",0,QObject::tr("Start position x"),QObject::tr(""),0,2000));

    return model;
}

PolygonGenerator::PolygonGenerator(QQuickItem *parent):WallpaperGenerator(parent)
{
    m_settings=new PolygonGeneratorSettings(this);
    connect(m_settings->activeModel()->sections()[0],&StandardItemModel::dataChanged, this, &WallpaperGenerator::redraw);
    connect(m_settings->activeModel()->sections()[1],&StandardItemModel::dataChanged, this, &PolygonGenerator::redraw);
    connect(m_settings->activeModel()->sections()[2],&StandardItemModel::dataChanged, this, &PolygonGenerator::redraw);

}

void PolygonGenerator::paint(QPainter *painter, double width, double height)
{
    QPen pen=painter->pen();
    painter->setBrush(QBrush(m_settings->backgroundColor()));
    painter->drawRect(QRectF(0,0,width, height));
    painter->setBrush(Qt::NoBrush);
    pen.setColor(m_settings->baseColor());
    pen.setWidthF(m_settings->stroke());
    painter->setRenderHint(QPainter::HighQualityAntialiasing);
    painter->setPen(pen);
    QPointF center(m_settings->getActiveData("Start position x").toDouble(),m_settings->getActiveData("Start position y").toDouble());
    double radius=m_settings->getActiveData("Radius").toDouble();
    double startAngle=m_settings->getActiveData("Angle").toDouble();
    double edges=m_settings->getActiveData("Edge number").toDouble();
    int opacity=m_settings->getActiveData("Opacity").toDouble();
    QPolygonF hexa=paintPolygon(edges,center.x(),center.y(),radius,startAngle);
    painter->drawPolygon(hexa);
    QList<QPointF> nextSpot;
    QList<QPointF> occupiedSpot;
    for(int i=0; i<hexa.size()-1; ++i){
        auto&& pt=hexa[i]-(hexa[i+1]-hexa[i])*m_settings->getActiveData("Spacing").toDouble();
        if(pt.x()>0 && pt.x()<width && pt.y()>0 && pt.y()<height){
             nextSpot.push_back(pt);
        }

    }
    auto && pt=hexa.back()-(hexa.front()-hexa.back())*m_settings->getActiveData("Spacing").toDouble();
    if(pt.x()>0 && pt.x()<width && pt.y()>0 && pt.y()<height){
         nextSpot.push_back(pt);
    }

    int i=0;
    double h;
    double s;
    double v;
    m_settings->baseColor().getHsvF(&h,&s,&v);
    while(i<m_settings->iterations()){
        if(nextSpot.isEmpty()){
            break;
        }
        ++i;
        int j=rand()%(nextSpot.size());


        center=nextSpot[j];
        nextSpot.removeAt(j);
        occupiedSpot.push_back(center);
        QPolygonF hexa=paintPolygon(edges,center.x(),center.y(),radius,startAngle);
       // if(i%2==0){
            double opacityAdjusted=opacity+rand()%50-25;
            if(opacityAdjusted<0){
                opacityAdjusted=0;
            }
            QColor color;
            if(m_settings->colorful()){
                color=QColor::fromHsvF(fmod(h+i*0.1,1.0f),s,v);
            }
            else{
                color=QColor::fromHsvF(h,s,v);
            }
            painter->setBrush(QColor::fromHsl(color.hslHue(), color.hslSaturation(),color.lightness(),opacityAdjusted));

       // }
       // else{
        //    painter->setBrush(Qt::NoBrush);
        //}
        painter->drawPolygon(hexa);
        qDebug()<<"Hexa: "<<hexa;
//                for(int i=0; i<hexa.size()-1; ++i){
//                   auto&& pt=hexa[i]+(hexa[i+1]-hexa[i])/2;
//                  // auto&& npt=center+(pt-center)*2;
//                   if(!nextSpot.contains(pt)&&!occupiedSpot.contains(pt)){
//                       nextSpot.push_back(pt);
//                   }
//                }

        for(int i=0; i<hexa.size()-1; ++i){
            auto&& pt=hexa[i]-(hexa[i+1]-hexa[i])*m_settings->getActiveData("Spacing").toDouble();
            if(pt.x()>0 && pt.x()<width && pt.y()>0 && pt.y()<height && !nextSpot.contains(pt)&&!occupiedSpot.contains(pt)){
                 nextSpot.push_back(pt);
            }
        }

        auto lastPt=hexa.back()-(hexa.front()-hexa.back())*m_settings->getActiveData("Spacing").toDouble();
        if(lastPt.x()>0 && lastPt.x()<width && lastPt.y()>0 && lastPt.y()<height &&!nextSpot.contains(lastPt)&&!occupiedSpot.contains(lastPt)){
            nextSpot.push_back(lastPt);
        }

    }
}


QPolygonF PolygonGenerator::paintPolygon(int edges, double centerX, double centerY, double radius, double alphaBegin)
{
    // double alphaBegin=std::acos(xStart);
    double dAlpha=M_PI*2/edges;
    double alpha=alphaBegin+dAlpha;

    QPolygonF polygon;

    while(alpha<=alphaBegin+2*M_PI){
        QPointF pt={centerX+cos(alpha)*radius, centerY+sin(alpha)*radius};
        alpha+=dAlpha;

        polygon.push_back(pt);
    }
    return polygon;
}
