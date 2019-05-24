#ifndef CALENDARLISTMANAGER_H
#define CALENDARLISTMANAGER_H

#include <QDate>
#include <QObject>

class Schedule : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int schId READ schId WRITE setSchId NOTIFY schIdChanged)
    Q_PROPERTY(int subId READ subId WRITE setSubId NOTIFY subIdChanged)
    Q_PROPERTY(int stageId READ stageId WRITE setStageId NOTIFY stageIdChanged)
    Q_PROPERTY(QString scheduleName READ scheduleName WRITE setScheduleName NOTIFY scheduleNameChanged)
    Q_PROPERTY(QString subScheduleName READ subScheduleName WRITE setSubScheduleName NOTIFY subScheduleNameChanged)
    Q_PROPERTY(QString stageTitle READ stageTitle WRITE setStageTitle NOTIFY stageTitleChanged)
    Q_PROPERTY(int status READ status WRITE setStatus NOTIFY statusChanged)

public:
    explicit Schedule(QObject *parent = nullptr): QObject(parent) {}

    int schId() { return m_schId; }
    void setSchId(int id)
    { if( id != m_schId ) { m_schId = id; emit schIdChanged(m_schId); } }

    int subId() { return m_subId; }
    void setSubId(int id)
    { if( id != m_subId ) { m_subId = id; emit subIdChanged(m_subId); } }

    int stageId() { return m_stageId; }
    void setStageId(int id)
    { if( id != m_stageId ) { m_stageId = id; emit stageIdChanged(m_stageId); } }

    QString scheduleName() { return m_scheduleName; }
    void setScheduleName(QString name)
    { if( name != m_scheduleName ) { m_scheduleName = name; emit scheduleNameChanged(m_scheduleName); } }

    QString subScheduleName() { return m_subScheduleName; }
    void setSubScheduleName(QString name)
    { if( name != m_subScheduleName ) { m_subScheduleName = name; emit subScheduleNameChanged(m_subScheduleName); } }

    QString stageTitle() { return m_stageTitle; }
    void setStageTitle(QString title)
    { if( title != m_stageTitle ) { m_stageTitle = title; emit stageTitleChanged(m_stageTitle); } }

    int status() { return m_status; }
    void setStatus(int status)
    { if( status != m_status ) { m_status = status; emit statusChanged(m_status); } }

signals:
    void schIdChanged(int id);
    void subIdChanged(int id);
    void stageIdChanged(int id);
    void scheduleNameChanged(QString name);
    void subScheduleNameChanged(QString name);
    void stageTitleChanged(QString title);
    void statusChanged(int status);

private:
    int m_schId;
    int m_subId;
    int m_stageId;
    QString m_scheduleName;
    QString m_subScheduleName;
    QString m_stageTitle;
    int m_status;
};

class CalendarListManager : public QObject
{
    Q_OBJECT
public:
    explicit CalendarListManager(QObject *parent = nullptr);

    Q_INVOKABLE bool isGivenDateExistGivenStatus(QDate date, int status);

    Q_INVOKABLE void updateCalendarQMLView() { emit update(); }

    Q_INVOKABLE QList<QObject*> schedulesForDate(const QDate &date);

signals:
    void update();

public slots:
};

#endif // CALENDARLISTMANAGER_H
