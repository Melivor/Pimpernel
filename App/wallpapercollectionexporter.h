#ifndef WALLPAPERCOLLECTIONEXPORTER_H
#define WALLPAPERCOLLECTIONEXPORTER_H
#include "wallpapergenerator.h"
#include <QObject>
class WallpaperCollectionExporter : public QThread
{
    Q_OBJECT
public:
    WallpaperCollectionExporter(WallpaperGenerator* generator, const QString& path, int width, int height);
    void run() override;

private:
    WallpaperGenerator* m_generator;
    QString m_path;
    int m_width;
    int m_height;

signals:
    void messageChanged(QString message);
    void progressChanged(int progress);
    void exportDone();
};

#endif // WALLPAPERCOLLECTIONEXPORTER_H
