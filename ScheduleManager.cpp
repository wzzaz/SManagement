#include "ScheduleManager.h"
#include "model/ScheduleModel.h"
#include "model/SubScheduleModel.h"
#include "data/ConnectionPool.h"

#include <QMessageBox>

ScheduleManager::ScheduleManager(QObject *pScheduleModel, QObject *pSubScheduleModel, QObject *parent)
    : QObject(parent)

{
    m_pScheduleModel    = static_cast<ScheduleModel*>(pScheduleModel);
    m_pSubScheduleModel = static_cast<SubScheduleModel*>(pSubScheduleModel);
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
    query.exec("SELECT * FROM schedule");
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
//    int id = m_pScheduleModel->data(index, ScheduleModel::IdRole).toInt();
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

    m_pScheduleModel->addSchedule(name, priority, id);
    emit scheduleAdded(m_pScheduleModel->size() - 1);
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
    QSqlDatabase db = ConnectionPool::openConnection();
    QSqlQuery query(db);
//    bool success = query.exec(QString("DELETE FROM sub_schedule WHERE id=%1")
//                              .arg(subId));
    bool success = query.exec(QString("DELETE sub_schedule,stage FROM sub_schedule "
                                      "LEFT JOIN stage ON stage.sub_schedule_id=stage.id "
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

void ScheduleManager::selectStage(const int index)
{
    m_nStageIndex = index;
}

void ScheduleManager::addStage(const QDateTime date, const QString title, const QString details, const QString result)
{
    QString addDate = QDateTime::currentDateTime().toString( "yyyy-MM-dd HH:mm:ss" );
    int subId = m_pSubScheduleModel->selectSubSchedule(m_nSubScheduleIndex)->id;
    int stageId;

    QSqlDatabase db = ConnectionPool::openConnection();
    QSqlQuery query(db);
    bool success = query.exec(QString("INSERT INTO stage(sub_schedule_id, date, title, details,result,updated_at,created_at) "
                                      "VALUES (%1,'%2','%3','%4','%5',%6,%7)")
                              .arg(subId).arg(date.toString("yyyy-MM-dd HH:mm:ss")).arg(title)
                              .arg(details).arg(result)
                              .arg(addDate).arg(addDate)
                              );
    query.exec("select @@IDENTITY");
    if( query.next() )
        stageId = query.value(0).toInt();
    ConnectionPool::closeConnection(db);

    if( !success )
        return;

    StageModel *stageModel = m_pSubScheduleModel->selectStageModel(m_nSubScheduleIndex);
    if( stageModel == nullptr )
        return;

    stageModel->addStage(date,title,details,result,stageId);
}

bool ScheduleManager::editStageDate(const int stageId, const QDateTime date)
{
    QString updateDate = QDateTime::currentDateTime().toString( "yyyy-MM-dd HH:mm:ss" );

    QSqlDatabase db = ConnectionPool::openConnection();
    QSqlQuery query(db);
    bool success = query.exec(QString("UPDATE stage SET date='%1', updated_at='%2' WHERE id=%3")
                              .arg(date.toString( "yyyy-MM-dd HH:mm:ss" )).arg(updateDate).arg(stageId));
    QString errText = query.lastError().text();
    ConnectionPool::closeConnection(db);

    if( !success ) {
        QMessageBox::warning(nullptr, tr("错误!"), errText);
        return false;
    }

    StageModel *stageModel = m_pSubScheduleModel->selectStageModel(m_nSubScheduleIndex);
    if( stageModel == nullptr )
        return false;

    stageModel->editDate(date, stageId);
    return true;
}

bool ScheduleManager::editStageTitle(const int stageId, const QString title)
{
    QString updateDate = QDateTime::currentDateTime().toString( "yyyy-MM-dd HH:mm:ss" );

    QSqlDatabase db = ConnectionPool::openConnection();
    QSqlQuery query(db);
    bool success = query.exec(QString("UPDATE stage SET title='%1', updated_at='%2' WHERE id=%3")
                              .arg(title).arg(updateDate).arg(stageId));
    QString errText = query.lastError().text();
    ConnectionPool::closeConnection(db);

    if( !success ) {
        QMessageBox::warning(nullptr, tr("错误!"), errText);
        return false;
    }

    StageModel *stageModel = m_pSubScheduleModel->selectStageModel(m_nSubScheduleIndex);
    if( stageModel == nullptr )
        return false;

    stageModel->editTitle(title, stageId);
    return true;
}

bool ScheduleManager::editStageDetails(const int stageId, const QString details)
{
    QString updateDate = QDateTime::currentDateTime().toString( "yyyy-MM-dd HH:mm:ss" );

    QSqlDatabase db = ConnectionPool::openConnection();
    QSqlQuery query(db);
    bool success = query.exec(QString("UPDATE stage SET details='%1', updated_at='%2' WHERE id=%3")
                              .arg(details).arg(updateDate).arg(stageId));
    QString errText = query.lastError().text();
    ConnectionPool::closeConnection(db);

    if( !success ) {
        QMessageBox::warning(nullptr, tr("错误!"), errText);
        return false;
    }

    StageModel *stageModel = m_pSubScheduleModel->selectStageModel(m_nSubScheduleIndex);
    if( stageModel == nullptr )
        return false;

    stageModel->editDetails(details, stageId);
    return true;
}

bool ScheduleManager::editStageResule(const int stageId, const QString result)
{
    QString updateDate = QDateTime::currentDateTime().toString( "yyyy-MM-dd HH:mm:ss" );

    QSqlDatabase db = ConnectionPool::openConnection();
    QSqlQuery query(db);
    bool success = query.exec(QString("UPDATE stage SET result='%1', updated_at='%2' WHERE id=%3")
                              .arg(result).arg(updateDate).arg(stageId));
    QString errText = query.lastError().text();
    ConnectionPool::closeConnection(db);

    if( !success ) {
        QMessageBox::warning(nullptr, tr("错误!"), errText);
        return false;
    }

    StageModel *stageModel = m_pSubScheduleModel->selectStageModel(m_nSubScheduleIndex);
    if( stageModel == nullptr )
        return false;

    stageModel->editResult(result, stageId);
    return true;
}

void ScheduleManager::removeStage(const int stageId)
{
    QSqlDatabase db = ConnectionPool::openConnection();
    QSqlQuery query(db);
    bool success = query.exec(QString("DELETE FROM stage WHERE id=%1")
                              .arg(stageId));
    QString errText = query.lastError().text();
    ConnectionPool::closeConnection(db);

    if( !success ) {
        return;
    }

    StageModel *stageModel = m_pSubScheduleModel->selectStageModel(m_nSubScheduleIndex);
    if( stageModel == nullptr )
        return;

    stageModel->removeStageWithId(stageId);
}

void ScheduleManager::unfoldSubScheduleAndStage(const int scheduleId)
{
    m_pSubScheduleModel->clear();

    QSqlDatabase db = ConnectionPool::openConnection();
    QSqlQuery query(db);
    query.exec(QString("SELECT sub.name name,sub.id subId,stg.date date,stg.title,"
                       "stg.details details,stg.result result,stg.id stgId FROM sub_schedule sub "
               "LEFT JOIN stage stg ON stg.sub_schedule_id=sub.id "
               "WHERE sub.schedule_id=%1 "
               "ORDER BY sub.order_no")
               .arg(scheduleId));
    while (query.next()) {
        QString name = query.value("name").toString();
        int subId    = query.value("subId").toInt();
        m_pSubScheduleModel->addSubSchedule(name, subId);

        QDateTime date  = query.value("date").toDateTime();
        QString title   = query.value("title").toString();
        QString details = query.value("details").toString();
        QString result  = query.value("result").toString();
        int stageId     = query.value("stgId").toInt();
        if( date.isValid() && stageId > 0 )
            m_pSubScheduleModel->addStage(name, date, title, details, result, stageId);

        qDebug() << "unfoldSubScheduleAndStage name=" << name << stageId << date.isValid();
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
