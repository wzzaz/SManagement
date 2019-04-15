#ifndef SCHEDULE_H
#define SCHEDULE_H

#include <QObject>
#include <QVector>

#include "SubSchedule.h"

class Schedule : public QObject
{
    Q_OBJECT
public:
    explicit Schedule(QString name, int priority, QObject *parent = nullptr);

    bool addSubSchedule(SubSchedule* subSchedule);

    bool removeSubSchedule(int index);

    bool editSubSchedule(int index, SubSchedule* subSchedule);

    SubSchedule* selectSubSchedule(int index);

    void clearSubSchedule();

    void editName(QString name) { m_name = name; }

    void editPriority(int priority) { m_priority = priority; }

    QString name() { return m_name; }

    int priority() { return m_priority; }

signals:

public slots:

private:

    QString m_name;
    int m_priority;

    QVector<SubSchedule*> m_subSchedules;
};

#endif // SCHEDULE_H
