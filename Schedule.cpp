#include "Schedule.h"

#define MAX_SUBSCHEDULE_NUMBER 10

Schedule::Schedule(QString name, int priority, QObject *parent)
    : QObject(parent),
      m_name(name),
      m_priority(priority)
{

}

bool Schedule::addSubSchedule(SubSchedule* subSchedule)
{
    if( m_subSchedules.size() < MAX_SUBSCHEDULE_NUMBER ) {
        m_subSchedules.append(subSchedule);
        return true;
    }

    return false;
}

bool Schedule::removeSubSchedule(int index)
{
    if( index < m_subSchedules.size() ) {
        SubSchedule* subSchedule = m_subSchedules.takeAt(index);
        delete subSchedule;
        subSchedule = NULL;
        return true;
    }

    return false;
}

bool Schedule::editSubSchedule(int index, SubSchedule* subSchedule)
{
    if( index < m_subSchedules.size() ) {
        m_subSchedules.replace(index, subSchedule);
        return true;
    }

    return false;
}

SubSchedule* Schedule::selectSubSchedule(int index)
{
    if( index < m_subSchedules.size() ) {
        return m_subSchedules.at(index);
    }

    return new SubSchedule();
}

void Schedule::clearSubSchedule()
{
    foreach(SubSchedule* subSchedule, m_subSchedules)
    {
        delete subSchedule;
        subSchedule = NULL;
    }
    m_subSchedules.clear();
}
