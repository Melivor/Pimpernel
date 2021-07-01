#include "parametricequationshapegenerator.h"
#include <QPainter>
#include "math.h"
#include <QTransform>
#include <QImageWriter>
#include <QPainterPath>
#include <QDir>
#include <QElapsedTimer>
#include <standarditemwithactions.h>

ParametricEquationsShapeGeneratorSettings::ParametricEquationsShapeGeneratorSettings(QObject* parent) : WallpaperGeneratorSettings(setupPrototype(), parent)
{
    auto xEquationItem=reinterpret_cast<ParametricEquationItem*>(item("Equation x"));
    if(xEquationItem){
        xEquationItem->setEquation(&m_xEquation);
    }
    auto yEquationItem=reinterpret_cast<ParametricEquationItem*>(item("Equation y"));
    if(yEquationItem){
        yEquationItem->setEquation(&m_yEquation);
    }
    connect(this, &StandardItemModelExplorer::activeSelectionChanged, this, &ParametricEquationsShapeGeneratorSettings::resetAnimator);
    m_animator.reset(new ParametricEquationAnimator(this));
    //resetAnimator();
}

void ParametricEquationsShapeGeneratorSettings::resetAnimator()
{
    m_animator->setParameters();
}
StandardItemModel* ParametricEquationsShapeGeneratorSettings::setupPrototype()
{
    StandardItemModel* mainModel=new StandardItemModel("ParametricEquationsShapeGenerator","ParametricEquationsShapeGenerator");
    mainModel->appendSection(ParametricEquationsShapeGeneratorSettings::setupStandardSection());
    mainModel->appendSection(setupStartValue());
    mainModel->appendSection(setupFunctionParameters());
    mainModel->appendSection(setupSpecificSection());
    return mainModel;
}

StandardItemModel* ParametricEquationsShapeGeneratorSettings::setupStandardSection()
{
    StandardItemModel* model=new StandardItemModel("Standard");
    model->appendRow(new StandardItem(QObject::tr("Pt number"),1000,"BoundIntNumberField.qml",0,QObject::tr("Number of pts"),QObject::tr(""),100,100000));
    model->appendRow(new StandardItem(QObject::tr("Base color"),QColor("grey"),"ColorField.qml",0,QObject::tr("Base color")));
    model->appendRow(new StandardItem(QObject::tr("Background color"),QColor("white"),"ColorField.qml",0,QObject::tr("White color")));
    model->appendRow(new StandardItem(QObject::tr("Stroke"),1,"BoundNumberField.qml",0,QObject::tr("Stroke"),QObject::tr(""),0,10));
    model->appendRow(new StandardItem(QObject::tr("Colorful"), true,"CheckBox.qml",0,tr("Colorful")));

    return model;
}

StandardItemModel* ParametricEquationsShapeGeneratorSettings::setupSpecificSection()
{
    StandardItemModel* model=new StandardItemModel("Transformations");
    model->appendRow(new StandardItem(QObject::tr("Scale"),20,"BoundNumberField.qml",0,QObject::tr("Scaling"),QObject::tr(""),0,100));
    model->appendRow(new StandardItem(QObject::tr("Rotation number"),4,"SliderInt.qml",0,QObject::tr("Number of rotation"),QObject::tr(""),1,10));
    model->appendRow(new StandardItem(QObject::tr("Angle"),0,"BoundNumberField.qml",0,QObject::tr("Angle of figure"),QObject::tr("degrees"),0,360));
    return model;
}

StandardItemModel* ParametricEquationsShapeGeneratorSettings::setupStartValue()
{
    StandardItemModel* model=new StandardItemModel("Start value");
    model->appendRow(new StandardItem(QObject::tr("Start position y"),0,"NumberField.qml",0,QObject::tr("Start position y"),QObject::tr(""),0,1));
    model->appendRow(new StandardItem(QObject::tr("Start position x"),0,"NumberField.qml",0,QObject::tr("Start position x"),QObject::tr(""),0,1));
    return model;
}

StandardItemModel* ParametricEquationsShapeGeneratorSettings::setupFunctionParameters()
{
    StandardItemModel* model=new StandardItemModel("Function parameters");
    auto xEquationItem=new ParametricEquationItem(nullptr,QObject::tr("Equation x"), 0,"TextFieldWithErrorWarning.qml",0,tr("Equation to use for x"));
    model->appendRow(xEquationItem);
    auto yEquationItem=new ParametricEquationItem(nullptr,QObject::tr("Equation y"), 0,"TextFieldWithErrorWarning.qml",0,tr("Equation to use for y"));
    model->appendRow(yEquationItem);
    //model->appendRow(new StandardItem(QObject::tr("Show equation"), true,"CheckBox.qml",0,tr("Show equation ?")));
    auto showEquationField=new StandardItemWithActions(QObject::tr("Show equation"), true,"CheckBox.qml",0,tr("Show equation ?"));
    auto equationFont=new StandardItem(QObject::tr("Font"), 0,"TextField.qml",0,tr("Font to use for equation"));
    auto equationFontSize=new StandardItem(QObject::tr("Font size"), 0,"NumberField.qml",0,tr("Font size to use for equation"));
    auto equationItemAction =[equationFont, equationFontSize](const QVariant& value, int role){
        if(role!=Qt::DisplayRole){
            return;
        }
        if(value.toBool()){
            equationFont->setEnabled(true);
            equationFontSize->setEnabled(true);
        }
        else{
            equationFont->setEnabled(false);
            equationFontSize->setEnabled(false);
        }
    };
    showEquationField->addAction(equationItemAction);
    model->appendRow(showEquationField);
    model->appendRow(equationFont);
    model->appendRow(equationFontSize);
    equationFont->setData("Segoe Script");
    equationFontSize->setData(12);
    auto startT=new StandardItem(QObject::tr("Start t"),0,"SliderMinMax.qml",-6,QObject::tr("Start value for t"),QObject::tr(""),-200,200);
    startT->setData(QVariantList({StandardItemModel::MinRole, StandardItemModel::MaxRole}),StandardItemModel::RolesTobeSaved);
    auto endT=new StandardItem(QObject::tr("End t"),0,"SliderMinMax.qml",0,QObject::tr("End value for t"),QObject::tr(""),-200,200);
    endT->setData(QVariantList({StandardItemModel::MinRole,StandardItemModel::MaxRole}),StandardItemModel::RolesTobeSaved);
    model->appendRow(startT);
    model->appendRow(endT);
    for(int i=1; i<=10; ++i){

        auto pItem=new ParametricEquationParameterItem(i);
        pItem->setData(QVariantList({StandardItemModel::MinRole, StandardItemModel::MaxRole}),StandardItemModel::RolesTobeSaved);
        /* hack to keep save min and max */

      //  auto maxItem=new StandardItemWithActions("p"+QString::number(i)+"max");

      //  maxItem->addAction(itemMaxAction);
       // auto minItem=new StandardItemWithActions("p"+QString::number(i)+"max");


      //  minItem->addAction(itemMinAction);
       // pItem->setMaxItem(maxItem);
        //pItem->setMinItem(minItem);

        xEquationItem->addParameterItem(pItem);
        yEquationItem->addParameterItem(pItem);
        model->appendRow(pItem);
        //model->appendRow(minItem);
         //model->appendRow(maxItem);
        pItem->setEnabled(false);
       // minItem->setEnabled(false);
        //maxItem->setEnabled(false);
    }
    return model;
}

QPointF ParametricEquationsShapeGeneratorSettings::pt(double t)
{
     return QPointF(m_xEquation.value(t)*getActiveData("Scale").toDouble(), m_yEquation.value(t)*getActiveData("Scale").toDouble());
}

ParametricEquationsShapeGenerator::ParametricEquationsShapeGenerator(QQuickItem *parent):WallpaperGenerator(parent)
{
   auto settings=new ParametricEquationsShapeGeneratorSettings(this);
   m_settings=settings;
    connect(m_settings->activeModel()->sections()[0],&StandardItemModel::dataChanged, this, &WallpaperGenerator::redraw);
    connect(m_settings->activeModel()->sections()[1],&StandardItemModel::dataChanged, this, &ParametricEquationsShapeGenerator::redraw);
    connect(m_settings->activeModel()->sections()[2],&StandardItemModel::dataChanged, this, &ParametricEquationsShapeGenerator::redraw);
    connect(m_settings->activeModel()->sections()[3],&StandardItemModel::dataChanged, this, &ParametricEquationsShapeGenerator::redraw);

}

void ParametricEquationsShapeGenerator::paint(QPainter *painter, double width, double height)
{
    QElapsedTimer timer;
    timer.start();
    QPen pen=painter->pen();
    painter->setBrush(QBrush(m_settings->backgroundColor()));
    painter->setPen(Qt::NoPen);
    painter->drawRect(QRectF(0,0,width, height));
    painter->setBrush(Qt::NoBrush);
    pen.setColor(m_settings->baseColor());
    pen.setWidthF(m_settings->stroke());
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setPen(pen);
    if(m_settings->getActiveData("Show equation").toBool()){
        drawLegend(painter, width);
    }
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
    double step=abs(m_settings->getActiveData("End t").toDouble()-m_settings->getActiveData("Start t").toDouble())/m_settings->getActiveData("Pt number").toInt();
    double angleStep=180/m_settings->getActiveData("Rotation number").toInt();
    double angle=0;
    int i=0;
    painter->translate(m_settings->getActiveData("Start position x").toDouble()*width, m_settings->getActiveData("Start position y").toDouble()*height);
    painter->rotate(m_settings->getActiveData("Angle").toDouble());
    QPolygonF polygon;
    polygon.reserve(m_settings->getActiveData("Pt number").toInt());
    double t=m_settings->getActiveData("Start t").toDouble();
    int count=0;
    int maxCount=m_settings->getActiveData("Pt number").toInt();
    auto settings=reinterpret_cast<ParametricEquationsShapeGeneratorSettings*>(m_settings);
    while(count<maxCount){
        polygon.append(settings->pt(t));
        t+=step;
        ++count;
    }
    while(angle<180)
    {
        if(m_settings->colorful()){
            pen.setColor(QColor::fromHsvF(fmod(h+i*0.1,1.0f),s,v));
            painter->setPen(pen);
            ++i;
        }
         angle+=angleStep;
         painter->rotate(angleStep);

         painter->drawPolyline(polygon);
    }
}


void ParametricEquationsShapeGenerator::drawLegend(QPainter* painter, double width)
{
    QFont font=painter->font();
    font.setFamily(m_settings->getActiveData("Font").toString());
    font.setPointSize(m_settings->getActiveData("Font size").toDouble());
    QFontMetrics fm(font);
    painter->setFont(font);
    QStringList legends;
    auto settings=reinterpret_cast<ParametricEquationsShapeGeneratorSettings*>(m_settings);
    legends.push_back(settings->xEquation());
    legends.push_back(settings->yEquation());
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
