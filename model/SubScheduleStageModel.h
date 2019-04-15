#ifndef SUBSCHEDULESTAGEMODEL_H
#define SUBSCHEDULESTAGEMODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QObject>
#include <QVector>

#include "SubSchedule.h"

class SubScheduleStageModel : public QAbstractItemModel
{
public:
    enum {
        NameRole = Qt::UserRole + 1,
        DateRole,
        DetailsRole,
        ResultRole
    };

    SubScheduleStageModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QModelIndex parent(const QModelIndex &index) const Q_DECL_OVERRIDE;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) Q_DECL_OVERRIDE;
//    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) Q_DECL_OVERRIDE;
//    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) Q_DECL_OVERRIDE;
//    bool hasChildren(const QModelIndex & parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QHash<int, QByteArray> roleNames() const Q_DECL_OVERRIDE;

    void initialize();

private:

    QVector<SubSchedule*> m_subScheduleData;
};

#endif // SUBSCHEDULESTAGEMODEL_H
