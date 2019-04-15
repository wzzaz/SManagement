#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QDebug>

#include "Schedule.h"
#include "model/ScheduleModel.h"
#include "model/SubScheduleModel.h"
#include "ScheduleManager.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

//    qmlRegisterUncreatableType<ScheduleModel, 1>("com.test.model", 1, 0,
//                                                   "ScheduleModel",
//                                                   "Cannot create ScheduleModel");
//    ScheduleModel *scheduleModel = new ScheduleModel();
//    engine.rootContext()->setContextProperty("scheduleModel", scheduleModel);

//    SubScheduleStageModel *subModel = new SubScheduleStageModel();
//    engine.rootContext()->setContextProperty("subScheduleStageModel", subModel);
//    subModel->initialize();

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

//    subSchModel->addSubSchedule("A",0);
//    subSchModel->addStage(0, QDateTime(QDate(),QTime()), "0", "", 1);
//    subSchModel->addStage(0, QDateTime(QDate(),QTime()), "1", "", 2);
//    subSchModel->addStage(0, QDateTime(QDate(),QTime()), "2", "", 3);



    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
