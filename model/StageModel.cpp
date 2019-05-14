#include "StageModel.h"
#include <QDebug>

StageModel::StageModel(QObject *parent)
    : QAbstractListModel(parent)
{
    m_stageData.clear();
}

StageModel::~StageModel()
{
    m_stageData.clear();
}

int StageModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_stageData.size();
}

int StageModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 1;
}

QVariant StageModel::data(const QModelIndex &index, int role) const
{
    if (index.row() >= 0 && index.row() < m_stageData.size() ) {
        if (role == DateRole) {
            StageStruct stage = m_stageData.at(index.row());
            return QVariant::fromValue(stage.dateTime);
        } else if (role == TitleRole) {
            StageStruct stage = m_stageData.at(index.row());
            return QVariant::fromValue(stage.title);
        } else if (role == DetailRole) {
            StageStruct stage = m_stageData.at(index.row());
            return QVariant::fromValue(stage.details);
        } else if (role == ResultRole) {
            StageStruct stage = m_stageData.at(index.row());
            return QVariant::fromValue(stage.result);
        } else if (role == StatusRole) {
            StageStruct stage = m_stageData.at(index.row());
            return QVariant::fromValue(stage.status);
        } else if (role == IdRole) {
            StageStruct stage = m_stageData.at(index.row());
            return QVariant::fromValue(stage.id);
        }
    }
    return QVariant(0);
}

bool StageModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.row() >= 0 && index.row() < m_stageData.size() ) {
        if (role == DateRole) {
            StageStruct stage = m_stageData.at(index.row());
            stage.dateTime = value.toDateTime();
            m_stageData.replace(index.row(), stage);
            return true;
        } else if (role == TitleRole ) {
            StageStruct stage = m_stageData.at(index.row());
            stage.title = value.toString();
            m_stageData.replace(index.row(), stage);
        } else if (role == DetailRole ) {
            StageStruct stage = m_stageData.at(index.row());
            stage.details = value.toString();
            m_stageData.replace(index.row(), stage);
        } else if (role == ResultRole ) {
            StageStruct stage = m_stageData.at(index.row());
            stage.result = value.toString();
            m_stageData.replace(index.row(), stage);
        }  else if (role == StatusRole ) {
            StageStruct stage = m_stageData.at(index.row());
            stage.status = value.toInt();
            m_stageData.replace(index.row(), stage);
        } else if (role == IdRole ) {
            StageStruct stage = m_stageData.at(index.row());
            stage.id = value.toInt();
            m_stageData.replace(index.row(), stage);
        }
    }
    return false;
}

QHash<int, QByteArray> StageModel::roleNames() const
{
    QHash<int, QByteArray> roles;

    roles[DateRole] = "date";
    roles[TitleRole] = "title";
    roles[DetailRole] = "details";
    roles[ResultRole] = "result";
    roles[StatusRole] = "status";
    roles[IdRole] = "id";
    return roles;
}

void StageModel::addStage(QDateTime date, QString title, QString details, QString result, int status,  int id)
{
    beginInsertRows(QModelIndex(), m_stageData.size(), m_stageData.size());
    m_stageData.append(StageStruct(date,title,details,result,status,id));
    endInsertRows();
}

int StageModel::insertStage(QDateTime date, QString title, QString details, QString result, int id)
{
    beginInsertRows(QModelIndex(), m_stageData.size(), m_stageData.size());
    m_stageData.append(StageStruct(date,title,details,result,id));
    endInsertRows();
    return orderStageWithDateTime(m_stageData.size() - 1, ASC);
}

void StageModel::removeStage(const int index)
{
    if( index >=0 && index < m_stageData.size() ) {
        beginRemoveRows(QModelIndex(), index, index);
        m_stageData.removeAt(index);
        endRemoveRows();
    }
}

void StageModel::removeStageWithId(const int id)
{
    int index = IdSearchStage(id);
    removeStage(index);
}

void StageModel::editStage(const int id, const QDateTime date, const QString title, const QString details, const QString result, const int status)
{
    int index = IdSearchStage(id);
    if ( index >= 0 && index < m_stageData.size() ) {
        StageStruct stage = m_stageData.at(index);
        stage.dateTime = date;
        stage.title = title;
        stage.details = details;
        stage.result = result;
        stage.status = status;
        m_stageData.replace(index, stage);
        emit dataChanged(createIndex(index, 0), createIndex(index, 0));
        orderStageWithDateTime(index, ASC);
    }
}

void StageModel::clear()
{
    beginResetModel();
    m_stageData.clear();
    endResetModel();
}

int StageModel::IdSearchStage(const int id)
{
    int index = -1;
    foreach( StageStruct stage, m_stageData) {
        index++;
        if (stage.id == id) {
            return index;
        }
    }
    return -1;
}

int StageModel::orderStageWithDateTime(const int index, Order order)
{
    if( index < 0 || index >= m_stageData.size() )
        return -1;

    QDateTime orderedDateTime = m_stageData.at(index).dateTime;
    int curIndex = m_stageData.size() - 1;
    for ( int i = 0; i < m_stageData.size(); ++i ) {
        if( i == index )
            continue;

        QDateTime curDateTime = m_stageData.at(i).dateTime;
        if( order == DESC )
        {
            if( orderedDateTime >= curDateTime ) {
                qDebug() << "StageModel::orderStageWithDateTime index=" << index << "i=" << i;
                curIndex = i > index ? i - 1 : i;
                break;
            }
        }
        else if ( order == ASC )
        {
            if( orderedDateTime <= curDateTime ) {
                qDebug() << "StageModel::orderStageWithDateTime index=" << index << "i=" << i;
                curIndex = i > index ? i - 1 : i;
                break;
            }
        }
    }
    qDebug() << "StageModel::orderStageWithDateTime index=" << index << "curIndex=" << curIndex;
    if( index == curIndex )
        return curIndex;

    beginMoveRows(QModelIndex(), index, index, QModelIndex(),
                  index > curIndex ? curIndex : curIndex + 1);
    m_stageData.move(index,curIndex);
    endMoveRows();
    return curIndex;
}
