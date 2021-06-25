#include "wallpapergenerator.h"
#include <QImageWriter>
#include <QPainter>
#include <QDir>

WallpaperGeneratorSettings::WallpaperGeneratorSettings(StandardItemModel* prototype, QObject* parent):StandardItemModelExplorer(prototype, parent)
{

    getModelList();
    if(rowCount()<2){
        qDebug()<<__PRETTY_FUNCTION__<<": is empty!";
        QDir dir(":/Examples/"+activeModel()->root());
        QStringList strList=dir.entryList();
        for(auto str:strList){
            //if(!QFile::exists(path()+str)){
                QFile::copy(dir.path()+"/"+str,path()+str);
            //}
        }
        getModelList();

    }
    setActiveSelection(activeModel()->root());
}


StandardItemModel* WallpaperGeneratorSettings::setupStandardSection()
{
    StandardItemModel* model=new StandardItemModel("Standard");
    model->appendRow(new StandardItem(QObject::tr("Iterations"),20,"SliderInt.qml",0,QObject::tr("Number of iterations"),QObject::tr(""),0,1000));
    model->appendRow(new StandardItem(QObject::tr("Base color"),QColor("grey"),"ColorField.qml",0,QObject::tr("Base color")));
    model->appendRow(new StandardItem(QObject::tr("Background color"),QColor("white"),"ColorField.qml",0,QObject::tr("White color")));
    model->appendRow(new StandardItem(QObject::tr("Stroke"),1,"Slider.qml",0,QObject::tr("Stroke"),QObject::tr(""),0,10));
    model->appendRow(new StandardItem(QObject::tr("Colorful"), true,"CheckBox.qml",0,tr("Colorful")));

    return model;
}


WallpaperGenerator::WallpaperGenerator(QQuickItem* parent) : QQuickPaintedItem(parent)
{

}


void WallpaperGenerator::paint(QPainter *painter)
{
    paint(painter, width(), height());
}


void WallpaperGenerator::save(const QString& name)
{
    QString pname=m_settings->activeModel()->name();
    saveAsPng(pngPath(name),500,300);
    m_settings->activeModel()->setName(name);
    m_settings->activeModel()->saveAsXml();

    m_settings->getModelList();
    m_settings->activeModel()->setName(pname);

}

void WallpaperGenerator::saveAsPng(QUrl url, int width, int height)
{

    QImage image(width, height, QImage::Format_ARGB32);
        image.fill(QColor(m_settings->backgroundColor()));

    QPainter painter;
    painter.begin(&image);
    double scale=std::min(image.width()/this->width(), image.height()/this->height());
    //double scale=width/this->width();
    painter.scale(scale, scale);
    paint(&painter, image.width()/scale, image.height()/scale);
    painter.end();
    QImageWriter writer(url.toLocalFile());
    if(!writer.write(image)){
        qWarning()<<writer.errorString();
    }
}
