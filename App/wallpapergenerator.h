 #ifndef WALLPAPERGENERATOR_H
#define WALLPAPERGENERATOR_H
#include "standarditemmodelexplorer.h"
#include "QQuickPaintedItem"
#include <QThread>

class WallpaperGeneratorSettings : public StandardItemModelExplorer
{
    Q_OBJECT
    //Q_PROPERTY()
public:
    WallpaperGeneratorSettings(StandardItemModel* prototype, QObject* parent=nullptr);
    QColor baseColor(){return QColor(getActiveData("Base color").toString());}
    QColor backgroundColor(){return QColor(getActiveData("Background color").toString());}
    int iterations(){return getActiveData("Iterations").toInt();}
    double stroke(){return getActiveData("Stroke").toDouble();}
    bool colorful(){return getActiveData("Colorful").toBool();}
protected:
    //PatternPainter* m_patternPainter;
    StandardItemModel* setupStandardSection();

};

class WallpaperGenerator : public QQuickPaintedItem
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(StandardItemModelExplorer* settings READ settings CONSTANT)
    Q_PROPERTY(int progress READ progress NOTIFY progressChanged)
    Q_PROPERTY(int maxProgress READ maxProgress NOTIFY maxProgressChanged)
    Q_PROPERTY(QString message READ message NOTIFY messageChanged)
    QThread workerThread;
public:
    WallpaperGenerator(QQuickItem *parent);
    ~WallpaperGenerator();
    virtual void paint(QPainter* painter, double width, double height)=0;
    void paint(QPainter* painter) override;
    WallpaperGeneratorSettings* settings(){return m_settings;}
    void redraw(){update();}
    Q_INVOKABLE void saveAsPicture(QUrl url, int width=1920, int height=1080);
    Q_INVOKABLE void saveAsPng(QUrl url, int width=1920, int height=1080);
    Q_INVOKABLE void saveAsSvg(QUrl url, int width=1920, int height=1080);
    Q_INVOKABLE void save(const QString& name);
    Q_INVOKABLE void exportCollection(const QUrl& folderPath, int width=1920, int height=1080);
    Q_INVOKABLE QUrl pngPath(QString name){QString path=m_settings->path(); path+=name; path+=".png";QUrl url =QUrl::fromLocalFile(path); return url;}
    //void saveCollectionStep(int index, const QString &path, int width=1920, int height=1080);
    //void finishingSaveCollection();
    int progress() const {return m_progress;}
    int maxProgress() const {return m_maxProgress;}
    QString message() const {return m_message;}
    void setProgress(int newProgress){m_progress=newProgress; emit progressChanged();}
    void setMaxProgress(int newProgress){m_maxProgress=newProgress; emit maxProgressChanged();}
    void setMessage(QString newMessage){m_message=newMessage; emit messageChanged();}
    virtual WallpaperGenerator* copy()=0;
protected:
    WallpaperGeneratorSettings* m_settings;
    int m_progress=0;
    int m_maxProgress=0;
    QString m_message="";
    void setSaveCollectionMessage(const QString& currentFile);
signals:
    void progressChanged();
    void maxProgressChanged();
    void done();
    void messageChanged();

};


class ListOfGenerator:public QObject
{
    Q_OBJECT
    Q_PROPERTY(QList<QObject*> generators READ generators)
public:
    ListOfGenerator(QList<QObject*> generator);
    QList<QObject*> generators(){return m_generators;}
private:
    QList<QObject*> m_generators;
};

class MetaGenerator : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name CONSTANT)
public:
  //  MetaPainter(QObject *parent=nullptr) {QObject(parent);}
    MetaGenerator(QObject *parent=nullptr):QObject(parent){};
    virtual const QString name(){return "Meta painter";}
    ~MetaGenerator(){release();}
    Q_INVOKABLE QQuickPaintedItem* item() {return m_item.get();}
    Q_INVOKABLE virtual void init(){};
    Q_INVOKABLE virtual void release(){m_item.reset();}
protected:
    std::unique_ptr<WallpaperGenerator> m_item;
};

#endif // WALLPAPERGENERATOR_H
