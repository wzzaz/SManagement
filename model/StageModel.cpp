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
    qDebug() << "~StageModel()" << this;
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
    roles[IdRole] = "id";
    return roles;
}

void StageModel::addStage(QDateTime date, QString title, QString details, QString result, int id)
{
    beginInsertRows(QModelIndex(), m_stageData.size(), m_stageData.size());
    m_stageData.append(StageStruct(date,title,details,result,id));
    endInsertRows();
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

void StageModel::editDate(const int index, const QDateTime date)
{
    if ( index >=0 && index < m_stageData.size() ) {
        StageStruct stage = m_stageData.at(index);
        stage.dateTime = date;
        m_stageData.replace(index, stage);
        emit dataChanged(createIndex(index, 0), createIndex(index, 0));
    }
}

void StageModel::editDate(const QDateTime date, const int id)
{
    int index = IdSearchStage(id);
    editDate(index,date);
}

void StageModel::editTitle(const int index, const QString title)
{
    if ( index >=0 && index < m_stageData.size() ) {
        StageStruct stage = m_stageData.at(index);
        stage.title = title;
        m_stageData.replace(index, stage);
        emit dataChanged(createIndex(index, 0), createIndex(index, 0));
    }
}

void StageModel::editTitle(const QString title, const int id)
{
    int index = IdSearchStage(id);
    editTitle(index,title);
}

void StageModel::editDetails(const int index, const QString details)
{
    if ( index >=0 && index < m_stageData.size() ) {
        StageStruct stage = m_stageData.at(index);
        stage.details = details;
        m_stageData.replace(index, stage);
        emit dataChanged(createIndex(index, 0), createIndex(index, 0));
    }
}

void StageModel::editDetails(const QString details, const int id)
{
    int index = IdSearchStage(id);
    editDetails(index,details);
}

void StageModel::editResult(const int index, const QString result)
{
    if ( index >=0 && index < m_stageData.size() ) {
        StageStruct stage = m_stageData.at(index);
        stage.result = result;
        m_stageData.replace(index, stage);
        emit dataChanged(createIndex(index, 0), createIndex(index, 0));
    }
}

void StageModel::editResult(const QString result, const int id)
{
    int index = IdSearchStage(id);
    editResult(index, result);
}

void StageModel::clear()
{
    beginResetModel();
    qDebug() << "StageModel::clear()" << m_stageData.size();
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
