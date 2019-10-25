#ifndef SCHEDULEMANAGER_H
#define SCHEDULEMANAGER_H

#include <QMap>
#include <QTimer>
#include <QObject>
#include <QVector>
#include <QDateTime>
#include <QModelIndex>


class ScheduleModel;
class SubScheduleModel;
class StageModel;
class CommonManager;
class ScheduleManager : public QObject
{
    Q_OBJECT
public:
    ScheduleManager(QObject *pScheduleModel,
                    QObject *pSubScheduleModel,
                    QObject *pCommonManager,
                    QObject *parent = nullptr);
    ~ScheduleManager();

    Q_INVOKABLE void initialize();

    /*******Schedule*/
    Q_INVOKABLE void selectSchedule(const int index);

    Q_INVOKABLE void addSchedule(const QString name, const int priority);

    Q_INVOKABLE bool editScheduleName(const int schId, const QString name);

    bool editSchedulePriority(const int schId,const int priority);

    Q_INVOKABLE void removeSchedule(const int schId);

    Q_INVOKABLE void selectScheduleWithId(const int schId, const int subId, const int stageId);

    /*******SubSchedule*/
    Q_INVOKABLE void selectSubSchedule(const int index);

    Q_INVOKABLE void addSubSchedule(const QString name, const int order);

    Q_INVOKABLE bool editSubScheduleName(const int subId, const QString name);

    Q_INVOKABLE void removeSubSchedule(const int subId, const int order);

    Q_INVOKABLE void moveSubSchedule(const int fromId, const int toId);

    Q_INVOKABLE void selectSubScheduleWithId(const int subId, const int stageId);

    /*******Stage*/
    void selectStage(const int index);

    Q_INVOKABLE void insertStage(QDateTime date, QString title, QString details, QString result);

    Q_INVOKABLE bool editStage(const int stageId, QDateTime date, QString title, QString details, QString result);

    Q_INVOKABLE void removeStage(const int stageId);

    Q_INVOKABLE void selectStageWithId(const int stageId);



    /**************************************************/
    Q_INVOKABLE int messageBoxForQuestion(const QString showText);

    Q_INVOKABLE bool updateStageWorkStatus();

    Q_INVOKABLE void setStatusFilter(int status, bool checked);

public slots:
    void slot_updateStageWorkStatus();

signals:    
    void jumpToSchedule(int index, int subId, int stageId);
    void jumpToSubSchedule(int index, int stageId);
    void jumpToStage(int index);

    void scheduleAdded(int index);
    void subScheduleAdded(int index);
    void stageAdded(int index);

    void scheduleRemoved();
    void subScheduleRemoved();
    void stageRemoved();

    void subScheduleReset();

    void stageUpdateStatus(bool updating);

private:
    enum {
        Add,
        Minus
    };
    void unfoldSubScheduleAndStage(const int scheduleId);

    void updateBehindSubScheduleOrder(const int startOrder,const int unChangeId, const int type);

    void clockInitialize();

private:

    ScheduleModel *m_pScheduleModel;
    SubScheduleModel *m_pSubScheduleModel;

    CommonManager *m_pCommonManager;

    int m_nScheduleIndex;
    int m_nSubScheduleIndex;
    int m_nStageIndex;

    QTimer *m_pStatusManagerClock;
};

#endif // SCHEDULEMANAGER_H
