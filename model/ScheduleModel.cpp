#include "ScheduleModel.h"

ScheduleModel::ScheduleModel(QObject *parent)
    : QAbstractListModel(parent)
{
    m_scheduleData.clear();
}

int ScheduleModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_scheduleData.size();
}

int ScheduleModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 1;
}

QVariant ScheduleModel::data(const QModelIndex &index, int role) const
{
    if (index.row() >= 0 && index.row() < m_scheduleData.size() ) {
        if (role == NameRole) {
            ScheduleStruct schedule = m_scheduleData.at(index.row());
            return QVariant::fromValue(schedule.name);
        } else if (role == PriorityRole) {
            ScheduleStruct schedule = m_scheduleData.at(index.row());
            return QVariant::fromValue(schedule.priority);
        } else if (role == IdRole) {
            ScheduleStruct schedule = m_scheduleData.at(index.row());
            return QVariant::fromValue(schedule.id);
        }
    }
    return QVariant(0);
}

bool ScheduleModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.row() >= 0 && index.row() < m_scheduleData.size() ) {
        if (role == NameRole) {
            ScheduleStruct schedule = m_scheduleData.at(index.row());
            schedule.name = value.toString();
            m_scheduleData.replace(index.row(), schedule);
            return true;
        } else if (role == PriorityRole) {
            ScheduleStruct schedule = m_scheduleData.at(index.row());
            schedule.priority = value.toInt();
            m_scheduleData.replace(index.row(), schedule);
            return true;
        } else if (role == IdRole ) {
            ScheduleStruct schedule = m_scheduleData.at(index.row());
            schedule.id = value.toInt();
            m_scheduleData.replace(index.row(), schedule);
        }
    }
    return false;
}

QHash<int, QByteArray> ScheduleModel::roleNames() const
{
    QHash<int, QByteArray> roles;

    roles[NameRole] = "name";
    roles[PriorityRole] = "priority";
    roles[IdRole] = "id";
    return roles;
}

int ScheduleModel::NameSearchSchedule(const QString name)
{
    int index = -1;
    foreach( ScheduleStruct schedule, m_scheduleData) {
        index++;
        if (schedule.name == name) {
            return index;
        }
    }
    return -1;
}

int ScheduleModel::IdSearchSchedule(const int id)
{
    int index = -1;
    foreach( ScheduleStruct schedule, m_scheduleData) {
        index++;
        if (schedule.id == id) {
            return index;
        }
    }
    return -1;
}

void ScheduleModel::addSchedule(const QString name, const int priority, const int id)
{
    beginInsertRows(QModelIndex(), m_scheduleData.size(), m_scheduleData.size());
    m_scheduleData.append(ScheduleStruct(name,priority,id));
    endInsertRows();
}

void ScheduleModel::removeSchedule(const QString name)
{
    int index = NameSearchSchedule(name);
    removeSchedule(index);
}

void ScheduleModel::removeSchedule(const int index)
{
    if( index >=0 && index < m_scheduleData.size() ) {
        beginRemoveRows(QModelIndex(), index, index);
        m_scheduleData.removeAt(index);
        endRemoveRows();
    }
}

void ScheduleModel::removeScheduleWithId(const int id)
{
    int index = IdSearchSchedule(id);
    removeSchedule(index);
}

void ScheduleModel::editName(const int index, const QString name)
{
    if ( index >=0 && index < m_scheduleData.size() ) {
        ScheduleStruct schedule = m_scheduleData.at(index);
        schedule.name = name;
        m_scheduleData.replace(index, schedule);
        emit dataChanged(createIndex(index, 0), createIndex(index, 0));
    }
}

void ScheduleModel::editName(const QString srcName, const QString newName)
{
    int index = NameSearchSchedule(srcName);
    editName(index, newName);
}

void ScheduleModel::editName(const QString name, const int srcId)
{
    int index = IdSearchSchedule(srcId);
    editName(index, name);
}

void ScheduleModel::editPriority(const int index, const int priority)
{
    if ( index >=0 && index < m_scheduleData.size() ) {
        ScheduleStruct schedule = m_scheduleData.at(index);
        schedule.priority = priority;
        m_scheduleData.replace(index, schedule);
        emit dataChanged(createIndex(index, 0), createIndex(index, 0));
    }
}

void ScheduleModel::editPriority(const QString srcName, const int priority)
{
    int index = NameSearchSchedule(srcName);
    editPriority(index, priority);
}

void ScheduleModel::editPriorityWithId(const int id, const int priority)
{
    int index = IdSearchSchedule(id);
    editPriority(index, priority);
}

void ScheduleModel::clear()
{
    beginResetModel();
    m_scheduleData.clear();
    endResetModel();
}

ScheduleStruct ScheduleModel::selectSchedule(const int index)
{
    if( index >= 0 && index < m_scheduleData.size() ) {
        return m_scheduleData.at(index);
    }
    return ScheduleStruct();
}
