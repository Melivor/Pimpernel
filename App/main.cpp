#include <QApplication>
#include <QQmlApplicationEngine>
//#include "linepainter.h"
//#include "polygonpainter.h"
//#include "polygonpainter2.h"
//#include "barnsleyfern.h"
//#include <rainpainter.h>
#include "raingenerator.h"
#include "curvelinegenerator.h"
#include "polygongenerator.h"
#include "parametricequationshapegenerator.h"
#include <QQmlContext>
int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication app(argc, argv);
    app.setOrganizationName("2ms");
    app.setOrganizationDomain("2ms.tech");

//    MetaLinePainter line;
//    QList<QObject*> painterList= {
//        new MetaLinePainter(),
//        new MetaPolygonPainter(),
//        new MetaPolygonPainter2(),
//    };
        QList<QObject*> painterList= {
            new MetaRainGenerator(),
            new MetaParametricEquationsShapeGenerator(),
            new MetaCurveLineGenerator(),
            new MetaPolygonGenerator(),
        };
    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("painterList", QVariant::fromValue(painterList));
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
