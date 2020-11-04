#include "parametricequationshapegenerator.h"
#include <QPainter>
#include "math.h"
#include <QTransform>
#include "standarditemwithactions.h"
#include <QImageWriter>
#include <QPainterPath>
#include <QDir>
ParametricEquationsShapeGeneratorSettings::ParametricEquationsShapeGeneratorSettings(QObject* parent) : WallpaperGeneratorSettings(setupPrototype(), parent)
{



}


StandardItemModel* ParametricEquationsShapeGeneratorSettings::setupPrototype()
{
    StandardItemModel* mainModel=new StandardItemModel("ParametricEquationsShapeGenerator","ParametricEquationsShapeGenerator");
    mainModel->appendSection(ParametricEquationsShapeGeneratorSettings::setupStandardSection());
    mainModel->appendSection(setupStartValue());
    mainModel->appendSection(setupFunctionParameters());
    mainModel->appendSection(setupSpecificSection());
   // setActiveSelection("ParametricEquationsShapeGenerator");
    return mainModel;
}

StandardItemModel* ParametricEquationsShapeGeneratorSettings::setupStandardSection()
{
    StandardItemModel* model=new StandardItemModel("Standard");
    model->appendRow(new StandardItem(QObject::tr("Pt number"),1000,"BoundIntNumberField.qml",0,QObject::tr("Number of pts"),QObject::tr(""),100,100000));
    model->appendRow(new StandardItem(QObject::tr("Base color"),QColor("grey"),"ColorField.qml",0,QObject::tr("Base color")));
    model->appendRow(new StandardItem(QObject::tr("Background color"),QColor("white"),"ColorField.qml",0,QObject::tr("White color")));
    model->appendRow(new StandardItem(QObject::tr("Stroke"),1,"Slider.qml",0,QObject::tr("Stroke"),QObject::tr(""),0,10));
    model->appendRow(new StandardItem(QObject::tr("Colorful"), true,"CheckBox.qml",0,tr("Colorful")));

    return model;
}

StandardItemModel* ParametricEquationsShapeGeneratorSettings::setupSpecificSection()
{

    StandardItemModel* model=new StandardItemModel("Transformations");

    model->appendRow(new StandardItem(QObject::tr("Scale"),20,"Slider.qml",0,QObject::tr("Scaling"),QObject::tr(""),0,100));

    model->appendRow(new StandardItem(QObject::tr("Rotation number"),4,"SliderInt.qml",0,QObject::tr("Number of rotation"),QObject::tr(""),1,10));
    model->appendRow(new StandardItem(QObject::tr("Angle"),0,"Slider.qml",0,QObject::tr("Angle of figure"),QObject::tr("degrees"),0,360));

    return model;
}

StandardItemModel* ParametricEquationsShapeGeneratorSettings::setupStartValue()
{
    StandardItemModel* model=new StandardItemModel("Start value");
    model->appendRow(new StandardItem(QObject::tr("Start position y"),0,"Slider.qml",0,QObject::tr("Start position y"),QObject::tr(""),0,1));
    model->appendRow(new StandardItem(QObject::tr("Start position x"),0,"Slider.qml",0,QObject::tr("Start position x"),QObject::tr(""),0,1));

    return model;
}

StandardItemModel* ParametricEquationsShapeGeneratorSettings::setupFunctionParameters()
{
    StandardItemModel* model=new StandardItemModel("Function parameters");
    StandardItemWithActions* equationSettings=new StandardItemWithActions(QObject::tr("Equation"), 0,"ComboBox.qml",0,tr("Equation to use"),"",0,0,getEquationsName());
    //equationSettings->addAction(std::bind(&ParametricEquationsShapeGeneratorSettings::setDefaultFunctionParameters, this));
    model->appendRow(equationSettings);
    //model->appendRow(new StandardItem(QObject::tr("Equation"), 0,"ComboBox.qml",0,tr("Equation to use"),"",0,0,getEquationsName()));
    model->appendRow(new StandardItem(QObject::tr("Show equation"), true,"CheckBox.qml",0,tr("Show equation ?")));
    model->appendRow(new StandardItem(QObject::tr("Start t"),0,"NumberField.qml",-6,QObject::tr("Start value for t"),QObject::tr(""),-2000,2000));
    model->appendRow(new StandardItem(QObject::tr("End t"),0,"NumberField.qml",0,QObject::tr("End value for t"),QObject::tr(""),-2000,2000));
    model->appendRow(new StandardItem(QObject::tr("P1"),0,"NumberField.qml",-6,QObject::tr("Parameters 1"),QObject::tr(""),-2000,2000));
    model->appendRow(new StandardItem(QObject::tr("P2"),0,"NumberField.qml",0,QObject::tr("Parameters 2"),QObject::tr(""),-2000,2000));
    return model;

}

void ParametricEquationsShapeGeneratorSettings::setDefaultFunctionParameters()
{
    setActiveData("Start t",QVariant(parametricFunctions[getActiveData("Equation").toInt()].tmin));
    setActiveData("End t",QVariant(parametricFunctions[getActiveData("Equation").toInt()].tmax));
    setActiveData("P1",QVariant(parametricFunctions[getActiveData("Equation").toInt()].p1));
    setActiveData("P2",QVariant(parametricFunctions[getActiveData("Equation").toInt()].p2));

}
ParametricEquationsShapeGenerator::ParametricEquationsShapeGenerator(QQuickItem *parent):WallpaperGenerator(parent)
{
    m_settings=new ParametricEquationsShapeGeneratorSettings(this);
    populateEquations();
    connect(m_settings->activeModel()->sections()[0],&StandardItemModel::dataChanged, this, &WallpaperGenerator::redraw);
    connect(m_settings->activeModel()->sections()[1],&StandardItemModel::dataChanged, this, &ParametricEquationsShapeGenerator::redraw);
    connect(m_settings->activeModel()->sections()[2],&StandardItemModel::dataChanged, this, &ParametricEquationsShapeGenerator::redraw);
    connect(m_settings->activeModel()->sections()[3],&StandardItemModel::dataChanged, this, &ParametricEquationsShapeGenerator::redraw);

}

void ParametricEquationsShapeGenerator::populateEquations()
{
    for(auto function:parametricFunctions){
        m_parametricFunctions[function.equationString]=function;
    }
}
void ParametricEquationsShapeGenerator::paint(QPainter *painter, double width, double height)
{
    QPen pen=painter->pen();
    painter->setBrush(QBrush(m_settings->backgroundColor()));
    painter->drawRect(QRectF(0,0,width, height));
    painter->setBrush(Qt::NoBrush);
    pen.setColor(m_settings->baseColor());
    pen.setWidthF(m_settings->stroke());
    painter->setRenderHint(QPainter::HighQualityAntialiasing);
    painter->setPen(pen);
    QString function=getEquationsName()[m_settings->getActiveData("Equation").toInt()].toString();
    if(m_settings->getActiveData("Show equation").toBool()){
        QFont font=painter->font();
        font.setFamily("Amazone BT");
        font.setPointSize(16);
        QFontMetrics fm(font);
        painter->setFont(font);
        QStringList legends;
        legends.push_back(m_parametricFunctions[function].equationXString(m_settings->getActiveData("P1").toDouble(), m_settings->getActiveData("P2").toDouble()));
        legends.push_back(m_parametricFunctions[function].equationYString(m_settings->getActiveData("P1").toDouble(), m_settings->getActiveData("P2").toDouble()));
        legends.push_back(QString::number(m_settings->getActiveData("Start t").toDouble())+"<t<"+QString::number(m_settings->getActiveData("End t").toDouble())+", N="+QString::number(m_settings->getActiveData("Pt number").toInt()));
        legends.push_back(QString::number(m_settings->getActiveData("Rotation number").toInt())+" rotations");
        int rightMargin=0;
        for(auto& legend:legends){
            rightMargin=std::max(rightMargin,fm.horizontalAdvance(legend));
        }
        rightMargin+=30;
        double topMargin=30;
        for(auto& legend:legends){
            painter->drawText(QPointF(width-rightMargin, topMargin), legend);
            topMargin+=fm.height();
        }

    }

    //computeX=m_computeXFunctions.at(function);
    //computeY=m_computeYFunctions.at(function);
    computeX=std::bind(m_parametricFunctions[function].equationX, std::placeholders::_1, m_settings->getActiveData("P1").toDouble(),m_settings->getActiveData("P2").toDouble());
    computeY=std::bind(m_parametricFunctions[function].equationY, std::placeholders::_1, m_settings->getActiveData("P1").toDouble(),m_settings->getActiveData("P2").toDouble());

    double h;
    double s;
    double v;
    m_settings->baseColor().getHsvF(&h,&s,&v);
    if(m_settings->getActiveData("Rotation number").toInt()==0){
        qWarning()<<"Rotation number can't be set at 0 (min 1)";
        return;

    }
    if( m_settings->getActiveData("Pt number").toInt()==0)
    {
        qWarning()<<"Pt number can't be set at 0 (min 1)";
        return;

    }

    double step=abs(m_settings->getActiveData("End t").toDouble()-m_settings->getActiveData("Start t").toDouble())*m_settings->getActiveData("Rotation number").toInt()/m_settings->getActiveData("Pt number").toInt();


    //double t=m_settings->getActiveData("Start t").toDouble();//m_parametricFunctions[function].tmin;
    double angleStep=180/m_settings->getActiveData("Rotation number").toInt();


    double angle=0;
    int i=0;
    QTransform transform;
    transform.translate(m_settings->getActiveData("Start position x").toDouble()*width, m_settings->getActiveData("Start position y").toDouble()*height);
    transform.rotate(m_settings->getActiveData("Angle").toDouble());
    while(angle<180)
    {
        //QPainterPath path;
        QPolygonF polygon;
        if(m_settings->colorful()){
            pen.setColor(QColor::fromHsvF(fmod(h+i*0.1,1.0f),s,v));
            painter->setPen(pen);
            ++i;
        }
        transform.rotate(angleStep);
        angle+=angleStep;

        double t=m_settings->getActiveData("Start t").toDouble();
        //QPointF pt(computeX(t)*m_settings->getActiveData("Scale").toDouble(), computeY(t)*m_settings->getActiveData("Scale").toDouble());
        //path.moveTo(transform.map(pt));

        //t+=step;
        while(t<m_settings->getActiveData("End t").toDouble()){
            QPointF pt(computeX(t)*m_settings->getActiveData("Scale").toDouble(), computeY(t)*m_settings->getActiveData("Scale").toDouble());
            //path.lineTo(transform.map(pt));
            polygon.append(transform.map(pt));
            t+=step;
        }
         //painter->drawPath(path);
         painter->drawPolyline(polygon);
    }
}

