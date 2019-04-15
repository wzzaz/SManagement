#ifndef SCHEDULEMANAGER_H
#define SCHEDULEMANAGER_H

#include <QObject>
#include <QVector>
#include <QModelIndex>

class ScheduleModel;
class SubScheduleModel;
class StageModel;
class ScheduleManager : public QObject
{
    Q_OBJECT
public:
    ScheduleManager(QObject *pScheduleModel,
                    QObject *pSubScheduleModel,
                    QObject *parent = nullptr);
    ~ScheduleManager();

    void initialize();

    /// *** Schedule
    ///
    Q_INVOKABLE void selectSchedule(const int index);
    // void selectSchedule(const int index);
    // void selectSchedule(const QString name);

    void addSchedule(const QString name, const int priority);

    bool editScheduleName(const int schId, const QString name);
    // bool editScheduleName(const int index,const QString name);
    // bool editScheduleName(const QString srcName,const QString newName);

    bool editSchedulePriority(const int schId,const int priority);
    // bool editSchedulePriority(const int index,const int priority);
    // bool editSchedulePriority(const QString srcName,const int priority);

    void removeSchedule(const int schId);
    // void removeSchedule(const int index);
    // void removeSchedule(const QString name);

    /// SubSchedule
    ///
    Q_INVOKABLE void selectSubSchedule(const int index);
    // void selectSubSchedule(const int index);
    // void selectSubSchedule(const QString name);

    void addSubSchedule(const QString name);

    bool editSubScheduleName(const int subId, const QString name);

    void removeSubSchedule(const int subId);
    // void removeSubSchedule(const QString name);

    ///* Stage
    void selectStage(const int index);

    void addStage(const QDateTime date, const QString details, const QString result);

    bool editStageDate(const int stageId, const QDateTime date);

    bool editStageDetails(const int stageId, const QString details);

    bool editStageResule(const int stageId, const QString result);

    void removeStage(const int stageId);

signals:

private:
    void unfoldSubScheduleAndStage(const int scheduleId);

private:

    ScheduleModel *m_pScheduleModel;
    SubScheduleModel *m_pSubScheduleModel;

    int m_nScheduleIndex;
    int m_nSubScheduleIndex;
    int m_nStageIndex;
};

#endif // SCHEDULEMANAGER_H
