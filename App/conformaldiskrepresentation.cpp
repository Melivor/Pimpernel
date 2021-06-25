#include "conformaldiskrepresentation.h"
#include <QPainter>
#include <QPainterPath>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
ConformalDiskRepresentationSettings::ConformalDiskRepresentationSettings(QObject *parent):WallpaperGeneratorSettings(setupPrototype(), parent)
{

}

StandardItemModel* ConformalDiskRepresentationSettings::setupPrototype()
{
    StandardItemModel* mainModel=new StandardItemModel("ParametricEquationsShapeGenerator","ParametricEquationsShapeGenerator");
    mainModel->appendSection(ConformalDiskRepresentationSettings::setupStandardSection());
    mainModel->appendSection(setupStartValue());
    mainModel->appendSection(setupDiskParameters());
    mainModel->appendSection(setupSpecificSection());
   // setActiveSelection("ParametricEquationsShapeGenerator");
    return mainModel;
}

StandardItemModel* ConformalDiskRepresentationSettings::setupStandardSection()
{
    StandardItemModel* model=new StandardItemModel("Standard");
    model->appendRow(new StandardItem(QObject::tr("Pt number"),1000,"BoundIntNumberField.qml",0,QObject::tr("Number of pts"),QObject::tr(""),100,100000));
    model->appendRow(new StandardItem(QObject::tr("Base color"),QColor("grey"),"ColorField.qml",0,QObject::tr("Base color")));
    model->appendRow(new StandardItem(QObject::tr("Background color"),QColor("white"),"ColorField.qml",0,QObject::tr("White color")));
    model->appendRow(new StandardItem(QObject::tr("Stroke"),1,"BoundNumberField.qml",0,QObject::tr("Stroke"),QObject::tr(""),0,10));
    model->appendRow(new StandardItem(QObject::tr("Colorful"), true,"CheckBox.qml",0,tr("Colorful")));

    return model;
}

StandardItemModel* ConformalDiskRepresentationSettings::setupSpecificSection()
{

    StandardItemModel* model=new StandardItemModel("Transformations");

    model->appendRow(new StandardItem(QObject::tr("Scale"),20,"BoundNumberField.qml",0,QObject::tr("Scaling"),QObject::tr(""),0,100));

    model->appendRow(new StandardItem(QObject::tr("Rotation number"),4,"SliderInt.qml",0,QObject::tr("Number of rotation"),QObject::tr(""),1,10));
    model->appendRow(new StandardItem(QObject::tr("Angle"),0,"BoundNumberField.qml",0,QObject::tr("Angle of figure"),QObject::tr("degrees"),0,360));

    return model;
}

StandardItemModel* ConformalDiskRepresentationSettings::setupStartValue()
{
    StandardItemModel* model=new StandardItemModel("Start value");
    model->appendRow(new StandardItem(QObject::tr("Start position y"),0,"Slider.qml",0,QObject::tr("Start position y"),QObject::tr(""),0,1));
    model->appendRow(new StandardItem(QObject::tr("Start position x"),0,"Slider.qml",0,QObject::tr("Start position x"),QObject::tr(""),0,1));

    return model;
}

StandardItemModel* ConformalDiskRepresentationSettings::setupDiskParameters()
{
    StandardItemModel* model=new StandardItemModel("Function parameters");
     model->appendRow(new StandardItem(QObject::tr("Diameter"),100,"BoundNumberField.qml",0,QObject::tr("Diameter of the conformal disk"),QObject::tr("pixels"),0,4000));
    return model;

}

ConformalDiskRepresentation::ConformalDiskRepresentation(QQuickItem *parent):WallpaperGenerator(parent)
{
    m_settings=new ConformalDiskRepresentationSettings(this);
    //populateEquations();
    connect(m_settings->activeModel()->sections()[0],&StandardItemModel::dataChanged, this, &WallpaperGenerator::redraw);
    connect(m_settings->activeModel()->sections()[1],&StandardItemModel::dataChanged, this, &ConformalDiskRepresentation::redraw);
    connect(m_settings->activeModel()->sections()[2],&StandardItemModel::dataChanged, this, &ConformalDiskRepresentation::redraw);
    connect(m_settings->activeModel()->sections()[3],&StandardItemModel::dataChanged, this, &ConformalDiskRepresentation::redraw);
}

void ConformalDiskRepresentation::paint(QPainter *painter, double width, double height)
{
    QPen pen=painter->pen();
    painter->setBrush(QBrush(m_settings->backgroundColor()));
    painter->setPen(Qt::NoPen);
    painter->drawRect(QRectF(0,0,width, height));
    painter->setBrush(Qt::NoBrush);
    pen.setColor(m_settings->baseColor());
    pen.setWidthF(m_settings->stroke());
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setPen(pen);
    double diameter=m_settings->getActiveData("Diameter").toDouble();
    QPointF center=QPointF(m_settings->getActiveData("Start position x").toDouble()*width, m_settings->getActiveData("Start position y").toDouble()*height);
    painter->drawEllipse(center,diameter/2,diameter/2);
    squareTilling(painter,center,diameter);
}

void ConformalDiskRepresentation::squareTilling(QPainter *painter, const QPointF &center, double diameter)
{

   // bottomLeft=toConformalDisk(bottomLeft/diameter);
    //topRight=toConformalDisk(topRight/diameter);
    //painter->drawLine(center+bottomLeft*diameter, center+topRight*diameter);
    double maxIt=1;
    double squareWidth=0.1;
    double slideX=0;//-maxIt;
    double slideY=0;//-maxIt;

    while (slideX<maxIt) {
        while(slideY<maxIt){
            drawSquareInConformalDisk(painter, center,diameter,squareWidth/2,{slideX,slideY});
            slideY+=squareWidth;
        }
        drawSquareInConformalDisk(painter, center,diameter,squareWidth/2,{slideX,slideY});
        slideX+=squareWidth;
        slideY=0;//-maxIt;
    }


    //drawSquareInConformalDisk(painter, center,diameter,0.2,{0,0});
    //drawSquareInConformalDisk(painter, center, diameter, 0.2);

}


void ConformalDiskRepresentation::drawSquareInConformalDisk(QPainter *painter, const QPointF &center, double diameter, double squareWidth, const QPointF& squareCenter)
{
    using namespace ConformalDiskModel;
    QPointF topLeft=QPointF(-squareWidth,squareWidth)+squareCenter;
    QPointF topRight=QPointF(squareWidth,squareWidth)+squareCenter;
    QPointF bottomRight=QPointF(squareWidth,-squareWidth)+squareCenter;
    QPointF bottomLeft=QPointF(-squareWidth,-squareWidth)+squareCenter;
    QPointF start;
    QPointF end;
    double arcRadius;
    QPointF arcCenter;
    diameter/=2;

    //QPainterPath path;
    LinesToConformalDisk::get(topLeft, topRight,start,end,arcRadius,arcCenter);
    unitArcToArc(center, diameter, arcCenter,arcRadius,start, end);
    drawArc(painter,arcCenter,arcRadius,start, end);
    LinesToConformalDisk::get(topRight, bottomRight,start,end,arcRadius,arcCenter);
    unitArcToArc(center, diameter, arcCenter,arcRadius,start, end);
   // qDebug()<<"Start: "<<start<<" end:"<<end<<"radius: "<<arcRadius;
    drawArc(painter,arcCenter,arcRadius,end,start);
    LinesToConformalDisk::get(bottomRight, bottomLeft,start,end,arcRadius,arcCenter);
    unitArcToArc(center, diameter, arcCenter,arcRadius,start, end);
    drawArc(painter,arcCenter,arcRadius,start, end);
    LinesToConformalDisk::get(bottomLeft, topLeft,start,end,arcRadius,arcCenter);
    unitArcToArc(center, diameter, arcCenter,arcRadius,start, end);
    drawArc(painter,arcCenter,arcRadius,start, end);

}

void ConformalDiskRepresentation::unitArcToArc(const QPointF& center, double diameter, QPointF &arcCenter, double& arcRadius, QPointF &start, QPointF &end)
{
    arcCenter*=diameter;
    arcCenter+=center;
    arcRadius*=diameter;
    start*=diameter;
    end*=diameter;
    start+=center;
    end+=center;
}
void ConformalDiskRepresentation::drawArc(QPainter* painter, const QPointF& arcCenter, double arcRadius, const QPointF &start, const QPointF &end, bool dir)
{

    QRectF const rect(arcCenter.x() - arcRadius, arcCenter.y() - arcRadius, arcRadius * 2, arcRadius * 2);
    double const startAngle = 16*atan2(start.y() - arcCenter.y(), start.x() - arcCenter.x()) * 180.0 / M_PI;
    double const endAngle   = 16*atan2(end.y() - arcCenter.y(), end.x() - arcCenter.x()) * 180.0 / M_PI;
    double spanAngle = (endAngle - startAngle);
    if(spanAngle<16*360-spanAngle){
        painter->drawArc(rect,-startAngle,-spanAngle);
    }
    //spanAngle=std::min(spanAngle, 16*360-spanAngle);
   // painter->drawEllipse(arcCenter,arcRadius,arcRadius);
    //qDebug()<<"Start angle"<<startAngle<<"end angle:" <<endAngle<<"span: "<<-spanAngle<<rect;

    else{
        //qDebug()<<"Start angle"<<startAngle<<"end angle:" <<endAngle<<"span: "<<spanAngle<<rect;
        painter->drawArc(rect,endAngle,16*360-spanAngle);
    }

}
