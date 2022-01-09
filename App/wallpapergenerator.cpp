#include "wallpapergenerator.h"
#include <QImageWriter>
#include <QPainter>
#include <QDir>
#include <QSvgGenerator>
#include <wallpapercollectionexporter.h>
WallpaperGeneratorSettings::WallpaperGeneratorSettings(StandardItemModel* prototype, QObject* parent):StandardItemModelExplorer(prototype, parent)
{

    getModelList();
    if(rowCount()<2){
        QDir dir(":/Examples/"+activeModel()->root());
        QStringList strList=dir.entryList();
        for(const auto &str:strList){
            //if(!QFile::exists(path()+str)){
            QFile::copy(dir.path()+"/"+str,path()+str);
            //}
        }
        getModelList();

    }
    else if(!m_modelNames.contains(activeModel()->root())){
        const auto rootName=activeModel()->root()+".xml";
        const auto rootFileName=":/Examples/"+activeModel()->root()+"/"+rootName;
        qDebug()<<rootFileName;
        QFile::copy(rootFileName, path()+rootName);
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

WallpaperGenerator::~WallpaperGenerator()
{
    qDebug()<<__PRETTY_FUNCTION__<<": "<<m_settings->activeSelectionName();
    //m_settings->activeModel()->saveAsXml();
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

void WallpaperGenerator::saveAsPicture(QUrl url, int width, int height)
{
    QFileInfo fileInfo(url.toLocalFile());

    if(fileInfo.suffix()=="png"){
        saveAsPng(url, width, height);
    }
    else if(fileInfo.suffix()=="svg"){
        saveAsSvg(url, width, height);
    }
    return;
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

void WallpaperGenerator::saveAsSvg(QUrl url, int width, int height)
{

    QSvgGenerator generator;
    generator.setFileName(url.toLocalFile());
    generator.setSize(QSize(width, height));
    generator.setViewBox(QRect(0, 0, width, height));
    generator.setTitle(tr("SVG Generator Example Drawing"));
    generator.setDescription(tr("An SVG drawing created by the SVG Generator "
                                    "Example provided with Qt."));


    QImage image(width, height, QImage::Format_ARGB32);
    image.fill(QColor(m_settings->backgroundColor()));
    QPainter painter;
    painter.begin(&generator);
    double scale=std::min(image.width()/this->width(), image.height()/this->height());
    painter.scale(scale, scale);
    paint(&painter, image.width()/scale, image.height()/scale);
    painter.end();
}

void WallpaperGenerator::exportCollection(const QUrl &folderPath, int width, int height)
{

    auto copiedInstance=this->copy();
    WallpaperCollectionExporter* exporter=new WallpaperCollectionExporter(copiedInstance, folderPath.toLocalFile(), width, height);
    setProgress(0);
    setMaxProgress(m_settings->rowCount());
    //copiedInstance->setParent(exporter);
    //connect(exporter, &WallpaperCollectionExporter::exportDone, this, &MyObject::handleResults);
    connect(exporter, &WallpaperCollectionExporter::progressChanged, this, &WallpaperGenerator::setProgress);
    connect(exporter, &WallpaperCollectionExporter::messageChanged, this, &WallpaperGenerator::setMessage);
        connect(exporter, &WallpaperCollectionExporter::finished, exporter, &QObject::deleteLater);
        connect(exporter, &WallpaperCollectionExporter::finished, copiedInstance, &QObject::deleteLater);
         connect(exporter, &WallpaperCollectionExporter::finished, this, &WallpaperGenerator::done);

       exporter->start();
    /*
       connect(this, &WallpaperGenerator::saveCollectionStepDone, this, &WallpaperGenerator::saveCollectionStep);
       connect(this, &WallpaperGenerator::saveCollectionDone, this, &WallpaperGenerator::finishingSaveCollection);
       saveCollectionStep(0, folderPath.toLocalFile(), width, height);
       */
}

/*
void WallpaperGenerator::saveCollectionStep(int index, const QString& path, int width, int height)
{

    if(index>=m_settings->rowCount()){
        emit saveCollectionDone();
        return;
    }
    const QString name=m_settings->data(m_settings->index(index)).toString();

    m_settings->copyProfilSettings(index, false);
    auto subFolders=name.split(" - ");
    if(subFolders.size()>1){
        const auto imagePath=path+"/"+subFolders[0]+"/"+subFolders[1]+".png";
        QDir dir(path);
        dir.mkdir(subFolders[0]);
        saveAsPicture(QUrl::fromLocalFile(imagePath),width,height);
    }
    else{
        const auto imagePath=path+"/"+m_settings->data(m_settings->index(index)).toString()+".png";
        saveAsPicture(QUrl::fromLocalFile(imagePath),width,height);
    }
    m_progress+=1;
    setSaveCollectionMessage(m_settings->data(m_settings->index(index)).toString());
    emit saveCollectionStepDone(index+1, path, width, height);
}

void WallpaperGenerator::setSaveCollectionMessage(const QString &currentFile)
{
    m_message="Processing "+currentFile;
}
void WallpaperGenerator::finishingSaveCollection()
{
    disconnect(this, &WallpaperGenerator::saveCollectionStepDone, this, &WallpaperGenerator::saveCollectionStep);
    disconnect(this, &WallpaperGenerator::saveCollectionDone, this, &WallpaperGenerator::finishingSaveCollection);


}
*/
ListOfGenerator::ListOfGenerator(QList<QObject*> generator)
{
    m_generators=generator;
    for(auto&& gen:m_generators){
        gen->setParent(this);
    }
}
