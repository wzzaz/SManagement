#include "CalendarListManager.h"
#include "data/ConnectionPool.h"
#include "CommonManager.h"

#include <QMessageBox>


CalendarListManager::CalendarListManager(QObject *pCommonManager, QObject *parent) : QObject(parent)
{
    m_pCommonManager = static_cast<CommonManager*>(pCommonManager);
}

bool CalendarListManager::isGivenDateExistGivenStatus(QDate date, int status)
{
    if( !m_pCommonManager->isStatusChecked(status) )
        return false;

#ifdef MYSQL_DATA
    QString g_date_diff = QString("DATEDIFF('%1',date)").arg(date.toString("yyyy-MM-dd"));
#elif SQLITE_DATA
    QString g_date_diff = QString("STRFTIME('%Y%m%d','%1') - STRFTIME('%Y%m%d',date)").arg(date.toString("yyyy-MM-dd"));
#endif

    bool exist = false;
    QSqlDatabase db = ConnectionPool::openConnection();
    QSqlQuery query(db);
    bool success = query.exec(QString("SELECT * FROM stage where status_id = %1 and %2 = 0")
               .arg(status).arg(g_date_diff));
    QString errText = query.lastError().text();
    while (query.next()) {
        exist = true;
        break;
    }
//    qDebug() << __FUNCTION__ << __LINE__ << query.lastQuery() << exist;
    ConnectionPool::closeConnection(db);

    if( !success )
        QMessageBox::warning(nullptr, tr("错误!"), errText);

    return exist;
}

QList<QObject *> CalendarListManager::schedulesForDate(const QDate &date)
{    
#ifdef MYSQL_DATA
    QString g_date_diff = QString("DATEDIFF('%1',stg.date)").arg(date.toString("yyyy-MM-dd"));
#elif SQLITE_DATA
    QString g_date_diff = QString("STRFTIME('%Y%m%d','%1') - STRFTIME('%Y%m%d',stg.date)").arg(date.toString("yyyy-MM-dd"));
#endif


    QSqlDatabase db = ConnectionPool::openConnection();
    QSqlQuery query(db);
    bool success = query.exec(QString("SELECT sch.id sch_id, sub.id sub_id, stg.id stage_id, sch.name sch_name, sub.name sub_name, stg.title title, stg.status_id status "
                       "FROM schedule sch "
                       "LEFT JOIN sub_schedule sub ON sch.id=sub.schedule_id "
                       "LEFT JOIN stage stg ON sub.id=stg.sub_schedule_id "
                       "WHERE %1 = 0 AND (stg.status_id IN %2 OR stg.status_id IS NULL)")
                              .arg(g_date_diff).arg(m_pCommonManager->statusFilterToSQLCode()));
//    qDebug() << __FUNCTION__ << __LINE__ << query.lastQuery();

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
