#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQml>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QUrl resourceUrl(QStringLiteral("qrc:/CustomerModelSingleton.qml"));
    qmlRegisterSingletonType(resourceUrl, "my.customermodel.singleton", 1, 0, "CustomerModel");


    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
