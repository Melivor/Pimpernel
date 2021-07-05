#include <QApplication>
#include <QQmlApplicationEngine>
#include "parametricequationshapegenerator.h"
#include <QQmlContext>
#include <QQuickStyle>
int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication app(argc, argv);
    app.setOrganizationName("2ms");
    app.setOrganizationDomain("2ms.tech");
    app.setApplicationName("Pimpernel");
    //QQuickStyle::addStylePath("../qqc2-suru-style/qml/QtQuick/Controls.2/Suru");
    //QQuickStyle::setStyle("qqc2-suru");
    qDebug()<<"Availbale styles: "<<QQuickStyle::availableStyles();
    QQmlApplicationEngine* splashEngine=new QQmlApplicationEngine();
    splashEngine->load("qrc:/SplashScreen.qml");
    QObject* object=splashEngine->rootObjects().front();
    app.processEvents();
    splashEngine->deleteLater();
    object->setProperty("message", "loading ui...");
//    MetaLinePainter line;
//    QList<QObject*> painterList= {
//        new MetaLinePainter(),
//        new MetaPolygonPainter(),
//        new MetaPolygonPainter2(),
//    };
        QList<QObject*> painterList= {
            new MetaParametricEquationsShapeGenerator(),
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
