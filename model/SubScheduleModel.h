#ifndef SUBSCHEDULEMODEL_H
#define SUBSCHEDULEMODEL_H

#include <QAbstractListModel>
#include <QObject>

#include "StageModel.h"

typedef struct _subSchedule {
    QString name;
    int id;
    StageModel *stageModel;

    _subSchedule(QString s_name, int n_id = -1) {
        name = s_name;
        id = n_id;
    }

    _subSchedule() {
        name = "";
        id = -1;
    }
} SubScheduleStruct;

class SubScheduleModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum {
        NameRole = Qt::UserRole + 1,
        IdRole
    };

    SubScheduleModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    virtual QHash<int, QByteArray> roleNames() const;

    bool addSubSchedule(const QString name, const int id);

    void removeSubSchedule(const QString name);
    void removeSubSchedule(const int index);
    void removeSubScheduleWithId(const int id);

    void editName(const int index,const QString name);
    void editName(const QString srcName, const QString newName);
    void editName(const QString name, const int id);

    void addStage(const int index, QDateTime date, QString details, QString result, int id);
    void addStage(const QString subName, QDateTime date, QString details, QString result, int id);

    Q_INVOKABLE StageModel *selectStageModel(const int index);
    //Q_INVOKABLE StageModel* stageModel(const int index) { return m_subScheduleData.at(index).stageModel; }

    void clear();

    SubScheduleStruct selectSubSchedule(const int index);

private:
    int NameSearchSubSchedule(const QString name);
    int IdSearchSubSchedule(const int id);

private:
    QVector<SubScheduleStruct> m_subScheduleData;
};

#endif // SUBSCHEDULEMODEL_H
