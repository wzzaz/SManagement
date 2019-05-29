#include <QGuiApplication>
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>
#include <QFontDatabase>
#include <QDebug>
#include <QFont>

#include "model/ScheduleModel.h"
#include "model/SubScheduleModel.h"
#include "data/ConnectionPool.h"
#include "CommonManager.h"
#include "ScheduleManager.h"
#include "CalendarListManager.h"

int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication app(argc, argv);

    QQmlApplicationEngine engine;

    qmlRegisterUncreatableType<StageModel, 1>("com.stage.model", 1, 0,
                                                   "StageModel",
                                                   "Cannot create StageModel");

#ifdef SQLITE_DATA
    ConnectionPool::initializeSQLite();
#endif

    ScheduleModel *scheduleModel = new ScheduleModel();
    engine.rootContext()->setContextProperty("scheduleModel", scheduleModel);
    SubScheduleModel *subSchModel = new SubScheduleModel();
    engine.rootContext()->setContextProperty("subScheduleModel", subSchModel);

    CommonManager *commonManager = new CommonManager();
    engine.rootContext()->setContextProperty("commonManager", commonManager);

    ScheduleManager *scheduleManager = new ScheduleManager(scheduleModel, subSchModel,commonManager);
    engine.rootContext()->setContextProperty("scheduleManager", scheduleManager);
    scheduleManager->initialize();

    CalendarListManager *calendarListManager = new CalendarListManager(commonManager);
    engine.rootContext()->setContextProperty("calendarListManager", calendarListManager);

    QString fontDir = ":/font/SourceCodePro-Regular.ttf";
    int fontId = QFontDatabase::addApplicationFont(fontDir);
    QString myFontFamily("Consolas");
    if( fontId > -1 ) {
        QStringList fontList = QFontDatabase::applicationFontFamilies(fontId);
        if( fontList.size() > 0 )
            myFontFamily = QFontDatabase::applicationFontFamilies(fontId).at(0);
    }

    QFont font(myFontFamily);
    font.setPixelSize(15);
    app.setFont(font);

    QQuickStyle::setStyle("Material");


    engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
