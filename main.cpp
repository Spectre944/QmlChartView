#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QApplication>
#include <QQmlContext>

#include "spectrumsource.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv); //Here etc..

    //qmlRegisterType<dispatcher>("WinHeater", 1, 0, "disp");

    QQmlApplicationEngine engine;
    QQmlContext *context = engine.rootContext();

    const QUrl url(u"qrc:/main.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
        &app, [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        }, Qt::QueuedConnection);

    //SpectrumSource dataSource(&viewer);
    SpectrumSource spectrumSource;
    context->setContextProperty("ssContext", &spectrumSource);

    engine.load(url);

    return app.exec();
}
