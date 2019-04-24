#ifndef SCHEDULEMODEL_H
#define SCHEDULEMODEL_H

#include <QAbstractListModel>
#include <QObject>


typedef struct _schedule {
    QString name;
    int priority;
    int id;

    _schedule(QString s_name, int n_priority, int n_id = -1) {
        name = s_name;
        priority = n_priority;
        id = n_id;
    }

    _schedule() {
        name = "";
        priority = -1;
        id = -1;
    }
} ScheduleStruct;

class ScheduleModel : public QAbstractListModel
{
    Q_OBJECT
public:    
    enum {
        NameRole = Qt::UserRole + 1,
        PriorityRole,
        IdRole
    };

    ScheduleModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    virtual QHash<int, QByteArray> roleNames() const;

    void addSchedule(const QString name, const int priority, const int id);

    void removeSchedule(const QString name);
    void removeSchedule(const int index);
    void removeScheduleWithId(const int id);

    void editName(const int index,const QString name);
    void editName(const QString srcName, const QString newName);
    void editName(const QString name, const int srcId);

    void editPriority(const int index, const int priority);
    void editPriority(const QString srcName, const int priority);
    void editPriorityWithId(const int id, const int priority);

    void clear();

    ScheduleStruct selectSchedule(const int index);

    int size() { return m_scheduleData.size(); }

private:
    int NameSearchSchedule(const QString name);
    int IdSearchSchedule(const int id);

private:
    QVector<ScheduleStruct> m_scheduleData;
};

#endif // SCHEDULEMODEL_H
