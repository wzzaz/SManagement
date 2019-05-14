#include "SubScheduleModel.h"
#include <QDebug>
#include <QQmlEngine>

SubScheduleModel::SubScheduleModel(QObject *parent)
    : QAbstractListModel(parent)
{
    m_subScheduleData.clear();
}

int SubScheduleModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_subScheduleData.size();
}

int SubScheduleModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 1;
}

QVariant SubScheduleModel::data(const QModelIndex &index, int role) const
{
    if (index.row() >= 0 && index.row() < m_subScheduleData.size() ) {
        if (role == NameRole) {
            SubScheduleStruct *subSchedule = m_subScheduleData.at(index.row());
            return QVariant::fromValue(subSchedule->name);
        } else if (role == IdRole) {
            SubScheduleStruct *subSchedule = m_subScheduleData.at(index.row());
            return QVariant::fromValue(subSchedule->id);
        }
    }
    return QVariant(0);
}

bool SubScheduleModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.row() >= 0 && index.row() < m_subScheduleData.size() ) {
        if (role == NameRole) {
            SubScheduleStruct *subSchedule = m_subScheduleData.at(index.row());
            subSchedule->name = value.toString();
//            m_subScheduleData.replace(index.row(), subSchedule);
            return true;
        } else if (role == IdRole ) {
            SubScheduleStruct *subSchedule = m_subScheduleData.at(index.row());
            subSchedule->id = value.toInt();
//            m_subScheduleData.replace(index.row(), subSchedule);
        }
    }
    return false;
}

QHash<int, QByteArray> SubScheduleModel::roleNames() const
{
    QHash<int, QByteArray> roles;

    roles[NameRole] = "name";
    roles[IdRole] = "id";
    return roles;
}

int SubScheduleModel::NameSearchSubSchedule(const QString name)
{
    int index = -1;
    foreach( SubScheduleStruct *subSchedule, m_subScheduleData) {
        index++;
        if (subSchedule->name == name) {
            return index;
        }
    }
    return -1;
}

int SubScheduleModel::IdSearchSubSchedule(const int id)
{
    int index = -1;
    foreach( SubScheduleStruct *subSchedule, m_subScheduleData) {
        index++;
        if (subSchedule->id == id) {
            return index;
        }
    }
    return -1;
}

bool SubScheduleModel::addSubSchedule(const QString name, const int id)
{
    if( IdSearchSubSchedule(id) > -1 ) {
        return false;
    }

    beginInsertRows(QModelIndex(), m_subScheduleData.size(), m_subScheduleData.size());
    SubScheduleStruct *sub = new SubScheduleStruct(name,id);
    sub->stageModel = new StageModel();
    // qDebug() << "SubScheduleModel::addSubSchedule " << sub->stageModel;
    m_subScheduleData.append(sub);
    endInsertRows();

    return true;
}

bool SubScheduleModel::insertSubSchedule(const int index, const QString name, const int id)
{
    if( IdSearchSubSchedule(id) > -1 ) {
        return false;
    }
    beginInsertRows(QModelIndex(), index, index);
    SubScheduleStruct *sub = new SubScheduleStruct(name,id);
    sub->stageModel = new StageModel();
    m_subScheduleData.insert(index,sub);
    endInsertRows();

    return true;
}

void SubScheduleModel::removeSubSchedule(const QString name)
{
    int index = NameSearchSubSchedule(name);
    removeSubSchedule(index);
}

void SubScheduleModel::removeSubSchedule(const int index)
{
    if( index >=0 && index < m_subScheduleData.size() ) {
        beginRemoveRows(QModelIndex(), index, index);
        m_subScheduleData.removeAt(index);
        endRemoveRows();
    }
}

void SubScheduleModel::removeSubScheduleWithId(const int id)
{
    int index = IdSearchSubSchedule(id);
    removeSubSchedule(index);
}

void SubScheduleModel::editName(const int index, const QString name)
{
    if ( index >=0 && index < m_subScheduleData.size() ) {
        SubScheduleStruct *subSchedule = m_subScheduleData.at(index);
        subSchedule->name = name;
//        m_subScheduleData.replace(index, subSchedule);
        emit dataChanged(createIndex(index, 0), createIndex(index, 0));
    }
}

void SubScheduleModel::editName(const QString srcName, const QString newName)
{
    int index = NameSearchSubSchedule(srcName);
    editName(index, newName);
}

void SubScheduleModel::editName(const QString name, const int id)
{
    int index = IdSearchSubSchedule(id);
    editName(index, name);
}

StageModel *SubScheduleModel::selectStageModel(const int index)
{
    if ( index >=0 && index < m_subScheduleData.size() ) {
        StageModel *stageModel = m_subScheduleData.at(index)->stageModel;
        QQmlEngine::setObjectOwnership(stageModel, QQmlEngine::CppOwnership);
        return stageModel;
    }
    return nullptr;
}

void SubScheduleModel::addStage(const int index, QDateTime date, QString title, QString details, QString result, int status, int id)
{
    SubScheduleStruct *sub = m_subScheduleData.at(index);
    sub->stageModel->addStage(date, title, details, result, status, id);
//    m_subScheduleData.replace(index, sub);
}

void SubScheduleModel::addStage(const QString subName, QDateTime date, QString title, QString details, QString result, int status, int id)
{
    int index = NameSearchSubSchedule(subName);
    addStage(index,date,title,details,result,status,id);
}

bool SubScheduleModel::moveSubSchedule(const int fromId, const int toId)
{
    clearErrorText();
    if( fromId == toId ) {
        setErrorText(tr("交换的两项相同"));
        return false;
    }

    int fromIndex = IdSearchSubSchedule(fromId);
    int toIndex   = IdSearchSubSchedule(toId);
    qDebug() << "SubScheduleModel::moveSubSchedule fromIndex=" << fromIndex << "toIndex=" << toIndex << m_subScheduleData.size();
    if( fromIndex == -1 || toIndex == -1 ) {
        setErrorText(tr("交换的其中一项不存在"));
        return false;
    }

    beginMoveRows(QModelIndex(), fromIndex, fromIndex, QModelIndex(),
                  fromIndex > toIndex ? toIndex : toIndex + 1);
    m_subScheduleData.move(fromIndex, toIndex);
    endMoveRows();

    int secondFromIndex = IdSearchSubSchedule(toId);
    int secondToIndex   = fromIndex;
    if( secondFromIndex == secondToIndex || secondFromIndex == -1 ) {
        return true;
    }

    qDebug() << "SubScheduleModel::moveSubSchedule secondFromIndex=" << secondFromIndex << "fromIndex=" << fromIndex;
    beginMoveRows(QModelIndex(), secondFromIndex, secondFromIndex, QModelIndex(),
                  secondFromIndex > fromIndex ? fromIndex : fromIndex + 1);
    m_subScheduleData.move(secondFromIndex, fromIndex);
    endMoveRows();
    return true;
}

void SubScheduleModel::clear()
{
    beginResetModel();
    qDebug() << "SubScheduleModel::clear()" << m_subScheduleData.size();
    qDeleteAll(m_subScheduleData);
    m_subScheduleData.clear();

    endResetModel();
}

SubScheduleStruct* SubScheduleModel::selectSubSchedule(const int index)
{
    if( index >= 0 & index < m_subScheduleData.size() ) {
        return m_subScheduleData.at(index);
    }
    return new SubScheduleStruct();
}
