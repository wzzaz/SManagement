#include "ScheduleManager.h"
#include "model/ScheduleModel.h"
#include "model/SubScheduleModel.h"
#include "data/ConnectionPool.h"

#include <QStringList>
#include <QMessageBox>

static QString g_stage_time_stamp_diff  = QString("TIMESTAMPDIFF(HOUR,(SELECT CURRENT_TIMESTAMP()), stage.date)");
static QString g_cur_time_stamp_diff    = QString("TIMESTAMPDIFF(HOUR,stage.date, (SELECT CURRENT_TIMESTAMP()))");
static QString g_result_is_empty        = QString("CHAR_LENGTH(result) = 0");
static QString g_sql_stage_status_value = QString("CASE WHEN (%1>=72) then 4 WHEN (%1>=0) AND (%1<72) THEN 3 WHEN (%2>0) AND (%3) THEN 2 ELSE 1 END")
        .arg(g_stage_time_stamp_diff).arg(g_cur_time_stamp_diff).arg(g_result_is_empty);

#define STAGE_CLOCK_TIME 3600000    // 1 hour

ScheduleManager::ScheduleManager(QObject *pScheduleModel, QObject *pSubScheduleModel, QObject *parent)
    : QObject(parent)

{
    m_pScheduleModel    = static_cast<ScheduleModel*>(pScheduleModel);
    m_pSubScheduleModel = static_cast<SubScheduleModel*>(pSubScheduleModel);

    updateStageWorkStatus();
    clockInitialize();
    statusFilterInitialize();
}

ScheduleManager::~ScheduleManager()
{

}

void ScheduleManager::initialize()
{
    m_pScheduleModel->clear();
    m_pSubScheduleModel->clear();

    QSqlDatabase db = ConnectionPool::openConnection();
    QSqlQuery query(db);
    query.exec("SELECT * FROM schedule ORDER BY created_at DESC");
    while (query.next()) {

        QString name = query.value("name").toString();
        int priority = query.value("priority").toInt();
        int id       = query.value("id").toInt();
        m_pScheduleModel->addSchedule(name, priority, id);
    }
    ConnectionPool::closeConnection(db);
}

void ScheduleManager::selectSchedule(const int index)
{
    m_nScheduleIndex = index;
    int id = m_pScheduleModel->selectSchedule(index).id;
    unfoldSubScheduleAndStage(id);
}

void ScheduleManager::addSchedule(const QString name, const int priority)
{
    QString date = QDateTime::currentDateTime().toString( "yyyy-MM-dd HH:mm:ss" );
    int id;

    QSqlDatabase db = ConnectionPool::openConnection();
    QSqlQuery query(db);
    bool success = query.exec(QString("INSERT INTO schedule(name,priority,created_at,updated_at) "
               "VALUES ('%1', %2, '%3', '%4')")
               .arg(name).arg(priority).arg(date).arg(date));
    query.exec("select @@IDENTITY");
    if( query.next() )
        id = query.value(0).toInt();
    ConnectionPool::closeConnection(db);

    if( !success )
        return;

    m_pScheduleModel->insertSchedule(name, priority, id);
    emit scheduleAdded(0);
}

bool ScheduleManager::editScheduleName(const int schId, const QString name)
{
    QString date = QDateTime::currentDateTime().toString( "yyyy-MM-dd HH:mm:ss" );

    QSqlDatabase db = ConnectionPool::openConnection();
    QSqlQuery query(db);
    bool success = query.exec(QString("UPDATE schedule SET name='%1', updated_at='%2' WHERE id=%3")
                              .arg(name).arg(date).arg(schId));
    QString errText = query.lastError().text();
    ConnectionPool::closeConnection(db);

    if( !success ) {
        QMessageBox::warning(nullptr, tr("错误!"), errText);
        return false;
    }

    m_pScheduleModel->editName(name, schId);
    return true;
}

bool ScheduleManager::editSchedulePriority(const int schId, const int priority)
{
    QString date = QDateTime::currentDateTime().toString( "yyyy-MM-dd HH:mm:ss" );

    QSqlDatabase db = ConnectionPool::openConnection();
    QSqlQuery query(db);
    bool success = query.exec(QString("UPDATE schedule SET priority=%1, updated_at='%2' WHERE id=%3")
                              .arg(priority).arg(date).arg(schId));
    QString errText = query.lastError().text();
    ConnectionPool::closeConnection(db);

    if( !success ) {
        QMessageBox::warning(nullptr, tr("错误!"), errText);
        return false;
    }

    m_pScheduleModel->editPriorityWithId(schId, priority);
    return true;
}

void ScheduleManager::removeSchedule(const int schId)
{
    int flag = QMessageBox::question(nullptr, tr("提示"), tr("是否确认删除该项"), tr("确认"), tr("取消"));
    if( flag != 0 )
        return;

    QSqlDatabase db = ConnectionPool::openConnection();
    QSqlQuery query(db);
//    bool success = query.exec(QString("DELETE FROM schedule WHERE id=%1")
//                              .arg(schId));
    bool success = query.exec(QString("DELETE schedule,sub_schedule,stage FROM schedule "
                                      "LEFT JOIN sub_schedule ON sub_schedule.schedule_id=schedule.id "
                                      "LEFT JOIN stage ON stage.sub_schedule_id=sub_schedule.id WHERE schedule.id=%1")
                              .arg(schId));
    QString errText = query.lastError().text();
    ConnectionPool::closeConnection(db);

    if( !success ) {
        QMessageBox::warning(nullptr, tr("错误!"), errText);
        return;
    }

    m_pScheduleModel->removeScheduleWithId(schId);
    emit scheduleRemoved();
}

void ScheduleManager::selectScheduleWithId(const int schId, const int subId, int stageId)
{
    int index = m_pScheduleModel->indexOf(schId);
    emit jumpToSchedule(index, subId, stageId);
}

void ScheduleManager::selectSubSchedule(const int index)
{
    m_nSubScheduleIndex = index;
}

void ScheduleManager::addSubSchedule(const QString name, const int order)
{
    QString date = QDateTime::currentDateTime().toString( "yyyy-MM-dd HH:mm:ss" );
    int schId = m_pScheduleModel->selectSchedule(m_nScheduleIndex).id;
    int subId;

    QSqlDatabase db = ConnectionPool::openConnection();
    QSqlQuery query(db);
    bool success = query.exec(QString("INSERT INTO sub_schedule(name, order_no, schedule_id, created_at,updated_at) "
                                      "VALUES ('%1',%2, %3,'%4','%5')")
                              .arg(name).arg(order).arg(schId).arg(date).arg(date)
                              );
    QString errText = query.lastError().text() + "\n" + query.lastQuery();
    query.exec("select @@IDENTITY");
    if( query.next() )
        subId = query.value(0).toInt();
    ConnectionPool::closeConnection(db);
    qDebug() << "ScheduleManager::addSubSchedule subId:" << subId;

    if( !success ) {
        QMessageBox::warning(nullptr, tr("错误!"), errText);
        return;
    }

    updateBehindSubScheduleOrder(order, subId, Add);

//    m_pSubScheduleModel->addSubSchedule(name, subId);
    m_pSubScheduleModel->insertSubSchedule(order, name, subId);
    emit subScheduleAdded(order);
}

bool ScheduleManager::editSubScheduleName(const int subId, const QString name)
{
    QString date = QDateTime::currentDateTime().toString( "yyyy-MM-dd HH:mm:ss" );

    QSqlDatabase db = ConnectionPool::openConnection();
    QSqlQuery query(db);
    bool success = query.exec(QString("UPDATE sub_schedule SET name='%1', updated_at='%2' WHERE id=%3")
                              .arg(name).arg(date).arg(subId));
    QString errText = query.lastError().text();
    ConnectionPool::closeConnection(db);

    if( !success ) {
        QMessageBox::warning(nullptr, tr("错误!"), errText);
        return false;
    }

    m_pSubScheduleModel->editName(name, subId);
    return true;
}

void ScheduleManager::removeSubSchedule(const int subId, const int order)
{
    int flag = QMessageBox::question(nullptr, tr("提示"), tr("是否确认删除该项"), tr("确认"), tr("取消"));
    if( flag != 0 )
        return;

    QSqlDatabase db = ConnectionPool::openConnection();
    QSqlQuery query(db);
//    bool success = query.exec(QString("DELETE FROM sub_schedule WHERE id=%1")
//                              .arg(subId));
    bool success = query.exec(QString("DELETE sub_schedule,stage FROM sub_schedule "
                                      "LEFT JOIN stage ON stage.sub_schedule_id=sub_schedule.id "
                                      "WHERE sub_schedule.id=%1")
                              .arg(subId));
    QString errText = query.lastError().text();
    ConnectionPool::closeConnection(db);

    if( !success ) {
        QMessageBox::warning(nullptr, tr("错误!"), errText);
        return;
    }

    updateBehindSubScheduleOrder(order, subId, Minus);

    m_pSubScheduleModel->removeSubScheduleWithId(subId);
    emit subScheduleRemoved();
}

void ScheduleManager::moveSubSchedule(const int fromId, const int toId)
{
    QString date = QDateTime::currentDateTime().toString( "yyyy-MM-dd HH:mm:ss" );

    QSqlDatabase db = ConnectionPool::openConnection();
    db.transaction();
    QSqlQuery query(db);
    bool querySuccess = query.exec(QString("UPDATE sub_schedule SET order_no = "
                                           "(CASE WHEN id = '%1' "
                                           "THEN (SELECT t1.order_no FROM (SELECT order_no FROM sub_schedule WHERE id ='%2') t1) "
                                           "ELSE (SELECT t2.order_no FROM (SELECT order_no FROM sub_schedule WHERE id ='%1') t2) "
                                           "END), updated_at= '%3' WHERE id in ('%1','%2');")
                                   .arg(fromId).arg(toId).arg(date));
    QString errText = query.lastError().text();
    if( !querySuccess ) {
        QMessageBox::warning(nullptr, tr("错误!"), errText);
        ConnectionPool::closeConnection(db);
        return;
    }

    bool modelSuccess = m_pSubScheduleModel->moveSubSchedule(fromId, toId);
    if( modelSuccess ) {
        db.commit();
    } else {
        db.rollback();
        QMessageBox::warning(nullptr, tr("错误!"), m_pSubScheduleModel->errorText());
    }
    ConnectionPool::closeConnection(db);
}

void ScheduleManager::selectSubScheduleWithId(const int subId, const int stageId)
{
    int index = m_pSubScheduleModel->indexOf(subId);
    emit jumpToSubSchedule(index, stageId);
}

void ScheduleManager::selectStage(const int index)
{
    m_nStageIndex = index;
}

void ScheduleManager::insertStage(const QDateTime date, const QString title, const QString details, const QString result)
{
    QString addDate = QDateTime::currentDateTime().toString( "yyyy-MM-dd HH:mm:ss" );
    int subId = m_pSubScheduleModel->selectSubSchedule(m_nSubScheduleIndex)->id;
    int stageId;

    QSqlDatabase db = ConnectionPool::openConnection();
    QSqlQuery query(db);
    bool success = query.exec(QString("INSERT INTO stage(sub_schedule_id, date, title, details,result,status_id,updated_at,created_at) "
                                      "VALUES (%1,'%2','%3','%4','%5',%6,'%7','%8')")
                              .arg(subId).arg(date.toString("yyyy-MM-dd HH:mm:ss")).arg(title)
                              .arg(details).arg(result).arg(3)
                              .arg(addDate).arg(addDate)
                              );
    QString errText = query.lastError().text();
    query.exec("select @@IDENTITY");
    if( query.next() )
        stageId = query.value(0).toInt();
    ConnectionPool::closeConnection(db);

    if( !success ) {
        QMessageBox::warning(nullptr, tr("错误!"), errText);
        return;
    }

    StageModel *stageModel = m_pSubScheduleModel->selectStageModel(m_nSubScheduleIndex);
    if( stageModel == nullptr )
        return;

    int insertIndex = stageModel->insertStage(date,title,details,result,stageId);
    emit stageAdded(insertIndex);
}

bool ScheduleManager::editStage(const int stageId, QDateTime date, QString title, QString details, QString result)
{
    if( stageId == -1 ) {
        QMessageBox::warning(nullptr, tr("错误!"), tr("错误的阶段数据!"));
        return false;
    }

    int flag = QMessageBox::question(nullptr, tr("提示"), tr("是否确认应用修改"), tr("确认"), tr("取消"));
    if( flag != 0 )
        return false;

    QString updateDate = QDateTime::currentDateTime().toString( "yyyy-MM-dd HH:mm:ss" );
    int status = Utils::checkStageWorkStatus(date, result.isEmpty());
    qDebug() << "ScheduleManager::editStage status=" << status;

    QSqlDatabase db = ConnectionPool::openConnection();
    QSqlQuery query(db);
    bool success = query.exec(QString("UPDATE stage SET date='%1', title='%2', details='%3', result='%4', status_id=%5, updated_at='%6' WHERE id=%7")
                              .arg(date.toString( "yyyy-MM-dd HH:mm:ss" )).arg(title).arg(details)
                              .arg(result).arg(status).arg(updateDate).arg(stageId));
    QString errText = query.lastError().text();
    ConnectionPool::closeConnection(db);

    if( !success ) {
        QMessageBox::warning(nullptr, tr("错误!"), errText);
        return false;
    }

    StageModel *stageModel = m_pSubScheduleModel->selectStageModel(m_nSubScheduleIndex);
    if( stageModel == nullptr )
        return false;

    stageModel->editStage(stageId, date, title, details, result, status);
    return true;
}

void ScheduleManager::removeStage(const int stageId)
{
    if( stageId == -1 ) {
        QMessageBox::warning(nullptr, tr("错误!"), tr("错误的阶段数据!"));
        return;
    }

    int flag = QMessageBox::question(nullptr, tr("提示"), tr("是否确认删除该项"), tr("确认"), tr("取消"));
    if( flag != 0 )
        return;

    QSqlDatabase db = ConnectionPool::openConnection();
    QSqlQuery query(db);
    bool success = query.exec(QString("DELETE FROM stage WHERE id=%1")
                              .arg(stageId));
    QString errText = query.lastError().text();
    ConnectionPool::closeConnection(db);

    if( !success ) {
        QMessageBox::warning(nullptr, tr("错误!"), errText);
        return;
    }

    StageModel *stageModel = m_pSubScheduleModel->selectStageModel(m_nSubScheduleIndex);
    if( stageModel == nullptr )
        return;

    stageModel->removeStageWithId(stageId);
    emit stageRemoved();
}

void ScheduleManager::selectStageWithId(const int stageId)
{
    StageModel *stageModel = m_pSubScheduleModel->selectStageModel(m_nSubScheduleIndex);
    qDebug() << "ScheduleManager::selectStageWithId stageModel=" << stageModel;
    if( stageModel == nullptr )
        return;

    int index = stageModel->indexOf(stageId);
    qDebug() << "ScheduleManager::selectStageWithId index=" << index << m_nSubScheduleIndex;
    emit jumpToStage(index);
}

int ScheduleManager::messageBoxForQuestion(const QString showText)
{
    int flag = QMessageBox::question(nullptr, tr("提示"), showText, tr("确认"), tr("取消"));
    return flag;
}

bool ScheduleManager::updateStageWorkStatus()
{
    QSqlDatabase db = ConnectionPool::openConnection();
    QSqlQuery query(db);
    bool success = query.exec(QString("UPDATE stage SET status_id=%1").arg(g_sql_stage_status_value));
    QString errText = query.lastError().text();
    ConnectionPool::closeConnection(db);

    if( !success ) {
        QMessageBox::warning(nullptr, tr("错误!"), errText);
        return false;
    }

    selectSchedule(m_nScheduleIndex);
    return true;
}

void ScheduleManager::setStatusFilter(int status, bool checked)
{
    m_statusFilter.insert(status,checked);
    selectSchedule(m_nScheduleIndex);
}

void ScheduleManager::slot_updateStageWorkStatus()
{
    emit stageUpdateStatus(true);

    updateStageWorkStatus();

    emit stageUpdateStatus(false);
}

void ScheduleManager::unfoldSubScheduleAndStage(const int scheduleId)
{
    m_pSubScheduleModel->clear();

    QSqlDatabase db = ConnectionPool::openConnection();
    QSqlQuery query(db);
    query.exec(QString("SELECT sub.name name,sub.id subId,stg.date date,stg.title,"
                       "stg.details details,stg.result result,stg.id stgId, stg.status_id status FROM sub_schedule sub "
               "LEFT JOIN stage stg ON stg.sub_schedule_id=sub.id "
               "WHERE sub.schedule_id=%1 and (stg.status_id in %2 OR stg.status_id is null) "
               "ORDER BY sub.order_no ASC, stg.date ASC")
               .arg(scheduleId).arg(statusFilterToSQLCode()));
    while (query.next()) {
        QString name = query.value("name").toString();
        int subId    = query.value("subId").toInt();
        m_pSubScheduleModel->addSubSchedule(name, subId);

        QDateTime date  = query.value("date").toDateTime();
        QString title   = query.value("title").toString();
        QString details = query.value("details").toString();
        QString result  = query.value("result").toString();
        int status      = query.value("status").toInt();
        int stageId     = query.value("stgId").toInt();
        if( date.isValid() && stageId > 0 )
            m_pSubScheduleModel->addStage(name, date, title, details, result, status, stageId);

        // qDebug() << "unfoldSubScheduleAndStage name=" << name << stageId << date.isValid();
    }
    QString errText = query.lastError().text();
    qDebug() << "unfoldSubScheduleAndStage err=" << errText;
    ConnectionPool::closeConnection(db);
    emit subScheduleReset();
}

void ScheduleManager::updateBehindSubScheduleOrder(const int startOrder, const int unChangeId, const int type)
{
    QString updateType;
    if( type == Add ) {
        updateType = "+";
    } else if( type == Minus ) {
        updateType = "-";
    }
    int schId = m_pScheduleModel->selectSchedule(m_nScheduleIndex).id;
    QSqlDatabase db = ConnectionPool::openConnection();
    QSqlQuery query(db);
    bool success = query.exec(QString("UPDATE sub_schedule SET order_no = order_no %1 1 "
                                      "WHERE order_no>=%2 AND id<>%3 AND schedule_id=%4")
                              .arg(updateType).arg(startOrder).arg(unChangeId).arg(schId)
                              );
    QString errText = query.lastError().text();
    ConnectionPool::closeConnection(db);
    if( !success ) {
        QMessageBox::warning(nullptr, tr("错误!"), errText);
    }
}

void ScheduleManager::clockInitialize()
{
    m_pStatusManagerClock = new QTimer(this);

    connect(m_pStatusManagerClock, SIGNAL(timeout()), this, SLOT(slot_updateStageWorkStatus()));

    m_pStatusManagerClock->setInterval(STAGE_CLOCK_TIME);
    m_pStatusManagerClock->setSingleShot(false);
    m_pStatusManagerClock->start();
}

void ScheduleManager::statusFilterInitialize()
{
    m_statusFilter.clear();

    m_statusFilter.insert(status_done,true);
    m_statusFilter.insert(status_undone,true);
    m_statusFilter.insert(status_closeToExpire,true);
    m_statusFilter.insert(status_waiting,true);
}

QString ScheduleManager::statusFilterToSQLCode()
{
    QStringList filters;
    QMap<int,bool>::const_iterator i;
    for (i = m_statusFilter.constBegin(); i != m_statusFilter.constEnd(); ++i) {
        if ( i.value() )
            filters << QString::number(i.key());
    }

    return QString("(%1)").arg(filters.join(","));
}
