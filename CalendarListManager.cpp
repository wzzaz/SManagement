#include "CalendarListManager.h"
#include "data/ConnectionPool.h"

#include <QMessageBox>

CalendarListManager::CalendarListManager(QObject *parent) : QObject(parent)
{

}

bool CalendarListManager::isGivenDateExistGivenStatus(QDate date, int status)
{
    bool exist = false;
    QSqlDatabase db = ConnectionPool::openConnection();
    QSqlQuery query(db);
    query.exec(QString("SELECT * FROM stage where status_id = %1 and DATEDIFF('%2',date) = 0")
               .arg(status).arg(date.toString("yyyy-MM-dd")));
    while (query.next()) {
        exist = true;
        break;
    }
    ConnectionPool::closeConnection(db);

    return exist;
}

QList<QObject *> CalendarListManager::schedulesForDate(const QDate &date)
{
    QSqlDatabase db = ConnectionPool::openConnection();
    QSqlQuery query(db);
    bool success = query.exec(QString("SELECT sch.id sch_id, sub.id sub_id, stg.id stage_id, sch.name sch_name, sub.name sub_name, stg.title title, stg.status_id status "
                       "FROM schedule sch "
                       "LEFT JOIN sub_schedule sub ON sch.id=sub.schedule_id "
                       "LEFT JOIN stage stg ON sub.id=stg.sub_schedule_id "
                       "WHERE DATEDIFF('%1',stg.date) = 0").arg(date.toString("yyyy-MM-dd")));

    QList<QObject*> schedules;
    while (query.next()) {
        Schedule *schedule = new Schedule(this);
        schedule->setSchId(query.value("sch_id").toInt());
        schedule->setSubId(query.value("sub_id").toInt());
        schedule->setStageId(query.value("stage_id").toInt());
        schedule->setScheduleName(query.value("sch_name").toString());
        schedule->setSubScheduleName(query.value("sub_name").toString());
        schedule->setStageTitle(query.value("title").toString());
        schedule->setStatus(query.value("status").toInt());

        schedules.append(schedule);
    }
    QString errText = query.lastError().text();
    ConnectionPool::closeConnection(db);

    if( !success ) {
        QMessageBox::warning(nullptr, tr("错误!"), errText);
    }

    return schedules;
}
