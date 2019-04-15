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
        QMessageBox::warning(nullptr, tr("Warning!"), errText);
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
        QMessageBox::warning(nullptr, tr("Warning!"), errText);
        return false;
    }

    m_pScheduleModel->editPriorityWithId(schId, priority);
    return true;
}

void ScheduleManager::removeSchedule(const int schId)
{
    QSqlDatabase db = ConnectionPool::openConnection();
    QSqlQuery query(db);
    bool success = query.exec(QString("DELETE FROM schedule WHERE id=%1")
                              .arg(schId));
    QString errText = query.lastError().text();
    ConnectionPool::closeConnection(db);

    if( !success ) {
        return;
    }

    m_pScheduleModel->removeScheduleWithId(schId);
}

void ScheduleManager::selectSubSchedule(const int index)
{
    m_nSubScheduleIndex = index;
}

void ScheduleManager::addSubSchedule(const QString name)
{
    QString date = QDateTime::currentDateTime().toString( "yyyy-MM-dd HH:mm:ss" );
    int schId = m_pScheduleModel->selectSchedule(m_nScheduleIndex).id;
    int subId;

    QSqlDatabase db = ConnectionPool::openConnection();
    QSqlQuery query(db);
    bool success = query.exec(QString("INSERT INTO sub_schedule(name, schedule_id, created_at,updated_at) "
                                      "VALUES ('%1',%2,%3,%4)")
                              .arg(name).arg(schId).arg(date).arg(date)
                              );
    query.exec("select @@IDENTITY");
    if( query.next() )
        subId = query.value(0).toInt();
    ConnectionPool::closeConnection(db);

    if( !success )
        return;

    m_pSubScheduleModel->addSubSchedule(name, subId);
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
        QMessageBox::warning(nullptr, tr("Warning!"), errText);
        return false;
    }

    m_pSubScheduleModel->editName(name, subId);
    return true;
}

void ScheduleManager::removeSubSchedule(const int subId)
{
    QSqlDatabase db = ConnectionPool::openConnection();
    QSqlQuery query(db);
    bool success = query.exec(QString("DELETE FROM sub_schedule WHERE id=%1")
                              .arg(subId));
    QString errText = query.lastError().text();
    ConnectionPool::closeConnection(db);

    if( !success ) {
        return;
    }

    m_pSubScheduleModel->removeSubScheduleWithId(subId);
}

void ScheduleManager::selectStage(const int index)
{
    m_nStageIndex = index;
}

void ScheduleManager::addStage(const QDateTime date, const QString details, const QString result)
{
    QString addDate = QDateTime::currentDateTime().toString( "yyyy-MM-dd HH:mm:ss" );
    int subId = m_pSubScheduleModel->selectSubSchedule(m_nSubScheduleIndex).id;
    int stageId;

    QSqlDatabase db = ConnectionPool::openConnection();
    QSqlQuery query(db);
    bool success = query.exec(QString("INSERT INTO stage(sub_schedule_id, date, details,result,updated_at,created_at) "
                                      "VALUES (%1,'%2','%3','%4',%5,%6)")
                              .arg(subId).arg(date.toString("yyyy-MM-dd HH:mm:ss")).arg(details).arg(result)
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

    stageModel->addStage(date,details,result,stageId);
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
        QMessageBox::warning(nullptr, tr("Warning!"), errText);
        return false;
    }

    StageModel *stageModel = m_pSubScheduleModel->selectStageModel(m_nSubScheduleIndex);
    if( stageModel == nullptr )
        return false;

    stageModel->editDate(date, stageId);
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
        QMessageBox::warning(nullptr, tr("Warning!"), errText);
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
        QMessageBox::warning(nullptr, tr("Warning!"), errText);
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
    query.exec(QString("SELECT sub.name name,sub.id subId,stg.date date,"
                       "stg.details details,stg.result result,stg.id stgId FROM sub_schedule sub "
               "LEFT JOIN stage stg ON stg.sub_schedule_id=sub.id "
               "WHERE sub.schedule_id=%1")
               .arg(scheduleId));
    while (query.next()) {
        QString name = query.value("name").toString();
        int subId    = query.value("subId").toInt();
        m_pSubScheduleModel->addSubSchedule(name, subId);

        QDateTime date  = query.value("date").toDateTime();
        QString details = query.value("details").toString();
        QString result  = query.value("result").toString();
        int stageId     = query.value("stgId").toInt();
        m_pSubScheduleModel->addStage(name, date, details, result, stageId);

        qDebug() << "unfoldSubScheduleAndStage name=" << name;
    }
    QString errText = query.lastError().text();
    qDebug() << "unfoldSubScheduleAndStage err=" << errText;
    ConnectionPool::closeConnection(db);
}
