#include "wallpapercollectionexporter.h"
#include <QDir>
WallpaperCollectionExporter::WallpaperCollectionExporter(WallpaperGenerator* generator, const QString &path, int width, int height): QThread(generator), m_generator(generator), m_path(path), m_width(width), m_height(height)
{

}

void WallpaperCollectionExporter::run()
{
    const auto settings=m_generator->settings();

    for(int index=0; index<settings->rowCount(); ++index){


        const QString name=settings->data(settings->index(index)).toString();

        settings->copyProfilSettings(index, false);
        auto subFolders=name.split(" - ");
        if(subFolders.size()>1){
            const auto imagePath=m_path+"/"+subFolders[0]+"/"+subFolders[1]+".png";
            QDir dir(m_path);
            dir.mkdir(subFolders[0]);
            m_generator->saveAsPicture(QUrl::fromLocalFile(imagePath),m_width,m_height);
        }
        else{
            const auto imagePath=m_path+"/"+settings->data(settings->index(index)).toString()+".png";
            m_generator->saveAsPicture(QUrl::fromLocalFile(imagePath),m_width,m_height);
        }
        emit messageChanged(name);
        emit progressChanged(index+1);
    }
    emit exportDone();
}
