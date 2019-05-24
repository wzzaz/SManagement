#ifndef STAGEMODEL_H
#define STAGEMODEL_H

#include <QAbstractListModel>
#include <QObject>
#include <QDateTime>

#include "Utils.h"

typedef struct _stage {
    QDateTime dateTime;
    QString title;
    QString details;
    QString result;
    int status;
    int id;

    _stage(QDateTime d_date, QString s_title, QString s_details, QString s_result, int n_id) {
        dateTime = d_date;
        title = s_title;
        details = s_details;
        result = s_result;
        status = status_closeToExpire;
        id = n_id;
    }

    _stage(QDateTime d_date, QString s_title, QString s_details, QString s_result, int n_status, int n_id) {
        dateTime = d_date;
        title = s_title;
        details = s_details;
        result = s_result;
        status = n_status;
        id = n_id;
    }

    _stage() {
        title = "";
        details = "";
        result = "";
        status = status_closeToExpire;
        id = -1;
    }
} StageStruct;

class StageModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum {
        DateRole = Qt::UserRole + 1,
        TitleRole,
        DetailRole,
        ResultRole,
        StatusRole,
        IdRole
    };

    StageModel(QObject *parent = nullptr);
    ~StageModel();

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    virtual QHash<int, QByteArray> roleNames() const;

    void addStage(QDateTime date, QString title, QString details, QString result, int status, int id);

    int insertStage(QDateTime date, QString title, QString details, QString result, int id);

    void removeStage(const int index);
    void removeStageWithId(const int id);

    void editStage(const int id, const QDateTime date, const QString title, const QString details, const QString result, const int status);

    void clear();

    int indexOf(int id);

private:
    typedef enum {
        ASC,
        DESC
    }Order;
    int IdSearchStage(const int id);
    int orderStageWithDateTime(const int index, Order order);

private:
    QVector<StageStruct> m_stageData;
};

#endif // STAGEMODEL_H
