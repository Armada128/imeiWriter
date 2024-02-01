#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QApplication>
#include <QIcon>
#include <QQuickStyle>
#include <QQmlContext>

#include "myserial.h"
#include "myfile.h"
#include "serialmovetothread.h"
#include "commodel.h"
#include "paritymodel.h"
#include "baudmodel.h"
#include "databitsmodel.h"
#include "stopbitsmodel.h"
#include "flowcontrolmodel.h"
#include "devicemodel.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    app.setWindowIcon(QIcon(":/CAAMS_icon.ico"));

    QQuickStyle::setStyle("Material");

    qmlRegisterType<SerialMoveToThread>("SerialMoveToThread", 1, 0, "SerialMoveToThread");
    qmlRegisterType<DeviceModel>("DeviceModel", 1, 0, "DeviceModel");
    comModel* ptr_comModel = new comModel;
    parityModel* ptr_parityModel = new parityModel;
    baudModel* ptr_baudModel = new baudModel;
    databitsModel* ptr_databitsModel = new databitsModel;
    stopbitsModel* ptr_stopbitsModel = new stopbitsModel;
    flowcontrolModel* ptr_flowcontrolModel = new flowcontrolModel;

    QQmlApplicationEngine engine;
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed,
        &app, []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);

    engine.rootContext()->setContextProperty("commodel", ptr_comModel);
    engine.rootContext()->setContextProperty("paritymodel", ptr_parityModel);
    engine.rootContext()->setContextProperty("baudmodel", ptr_baudModel);
    engine.rootContext()->setContextProperty("databitsmodel", ptr_databitsModel);
    engine.rootContext()->setContextProperty("stopbitsmodel", ptr_stopbitsModel);
    engine.rootContext()->setContextProperty("flowcontrolmodel", ptr_flowcontrolModel);

    engine.loadFromModule("imeiWriter", "Main");

    return app.exec();
}
