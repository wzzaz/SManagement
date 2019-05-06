#include <QGuiApplication>
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QDebug>
#include <QFont>

#include "model/ScheduleModel.h"
#include "model/SubScheduleModel.h"
#include "ScheduleManager.h"

int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication app(argc, argv);

    QQmlApplicationEngine engine;

    qmlRegisterUncreatableType<StageModel, 1>("com.stage.model", 1, 0,
                                                   "StageModel",
                                                   "Cannot create StageModel");

    ScheduleModel *scheduleModel = new ScheduleModel();
    engine.rootContext()->setContextProperty("scheduleModel", scheduleModel);
    SubScheduleModel *subSchModel = new SubScheduleModel();
    engine.rootContext()->setContextProperty("subScheduleModel", subSchModel);

    ScheduleManager *scheduleManager = new ScheduleManager(scheduleModel, subSchModel);
    engine.rootContext()->setContextProperty("scheduleManager", scheduleManager);
    scheduleManager->initialize();

    QFont font("Source Code Pro");
    font.setPixelSize(15);
    app.setFont(font);


    engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
