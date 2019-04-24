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

    Q_INVOKABLE void initialize();

    /// *** Schedule
    ///
    Q_INVOKABLE void selectSchedule(const int index);
    // void selectSchedule(const int index);
    // void selectSchedule(const QString name);

    Q_INVOKABLE void addSchedule(const QString name, const int priority);

    Q_INVOKABLE bool editScheduleName(const int schId, const QString name);
    // bool editScheduleName(const int index,const QString name);
    // bool editScheduleName(const QString srcName,const QString newName);

    bool editSchedulePriority(const int schId,const int priority);
    // bool editSchedulePriority(const int index,const int priority);
    // bool editSchedulePriority(const QString srcName,const int priority);

    Q_INVOKABLE void removeSchedule(const int schId);
    // void removeSchedule(const int index);
    // void removeSchedule(const QString name);

    /// SubSchedule
    ///
    Q_INVOKABLE void selectSubSchedule(const int index);
    // void selectSubSchedule(const int index);
    // void selectSubSchedule(const QString name);

    Q_INVOKABLE void addSubSchedule(const QString name, const int order);

    Q_INVOKABLE bool editSubScheduleName(const int subId, const QString name);

    Q_INVOKABLE void removeSubSchedule(const int subId, const int order);
    // void removeSubSchedule(const QString name);

    Q_INVOKABLE void moveSubSchedule(const int fromId, const int toId);

    ///* Stage
    void selectStage(const int index);

    void addStage(const QDateTime date, const QString title, const QString details, const QString result);

    bool editStageDate(const int stageId, const QDateTime date);

    bool editStageTitle(const int stageId, const QString title);

    bool editStageDetails(const int stageId, const QString details);

    bool editStageResule(const int stageId, const QString result);

    void removeStage(const int stageId);

signals:
    void scheduleAdded(int index);
    void subScheduleAdded(int index);
    void subScheduleReset();

private:
    enum {
        Add,
        Minus
    };
    void unfoldSubScheduleAndStage(const int scheduleId);
    void updateBehindSubScheduleOrder(const int startOrder,const int unChangeId, const int type);

private:

    ScheduleModel *m_pScheduleModel;
    SubScheduleModel *m_pSubScheduleModel;

    int m_nScheduleIndex;
    int m_nSubScheduleIndex;
    int m_nStageIndex;
};

#endif // SCHEDULEMANAGER_H
