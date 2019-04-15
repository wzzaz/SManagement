#include "SubScheduleStageModel.h"

#include <QDebug>

SubScheduleStageModel::SubScheduleStageModel(QObject *parent)
    :QAbstractItemModel(parent)
{

}

int SubScheduleStageModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_subScheduleData.size();
}

int SubScheduleStageModel::columnCount(const QModelIndex &parent) const
{
    SubSchedule *sub = m_subScheduleData.at(parent.row());
    return sub->stageSize() + 1;
}

QVariant SubScheduleStageModel::data(const QModelIndex &index, int role) const
{
    if( role == NameRole ) {
        SubSchedule *sub = m_subScheduleData.at(index.row());
        return QVariant::fromValue(sub->name());
    } else if( role == DateRole || role == DetailsRole || role == ResultRole ) {
        SubSchedule *sub = m_subScheduleData.at(index.row());
        if( index.column() > 0 && index.column() <= sub->stageSize() + 1 ) {
            Stage *stage = sub->selectStage(index.column() - 1);
            if( role == DateRole ) {
                return QVariant::fromValue(stage->date);
            } else if( role == DetailsRole ) {
                qDebug() << index;
                return QVariant::fromValue(stage->details);
            } else if( role == ResultRole ) {
                return QVariant::fromValue(stage->result);
            }
        }
    }

    return QVariant();
}

QModelIndex SubScheduleStageModel::index(int row, int column, const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    if( column == 0 ) {
        return createIndex(row, column);
    }

    return QModelIndex();
}

QModelIndex SubScheduleStageModel::parent(const QModelIndex &index) const
{
    if( index.column() > 0 ) {
        return createIndex(index.row(), 0);
    }

    return QModelIndex();
}

bool SubScheduleStageModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if( index.row() >= 0 && index.row() <= m_subScheduleData.size() ) {
        SubSchedule *sub = m_subScheduleData.at(index.row());
        if( role == NameRole ){
            sub->editName(value.toString());
            return true;
        } else if( role == DateRole || role == DetailsRole || role == ResultRole ) {
            if( index.column() >= 0 && index.column() <= sub->stageSize() + 1 ) {
                Stage *stage = sub->selectStage(index.column() - 1);
                if( role == DateRole ) {
                    stage->date = value.toDateTime();
                } else if( role == DetailsRole ) {
                    stage->details = value.toString();
                } else {
                    stage->result = value.toString();
                }
                return true;
            }
        }
    }
    return false;
}

QHash<int, QByteArray> SubScheduleStageModel::roleNames() const
{
    QHash<int, QByteArray> roles;

    roles[NameRole] = "name";
    roles[DateRole] = "date";
    roles[DetailsRole] = "details";
    roles[ResultRole] = "result";
    return roles;
}

void SubScheduleStageModel::initialize()
{
    SubSchedule *sub = new SubSchedule("A");
    Stage * stage = new Stage(QDateTime(QDate(), QTime()), "0");
    Stage * stage1 = new Stage(QDateTime(QDate(), QTime()), "1");
    Stage * stage2 = new Stage(QDateTime(QDate(), QTime()), "2");
    sub->addStage(stage);
    sub->addStage(stage1);
    sub->addStage(stage2);

    m_subScheduleData.append(sub);
}
