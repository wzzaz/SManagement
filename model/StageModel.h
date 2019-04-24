#ifndef STAGEMODEL_H
#define STAGEMODEL_H

#include <QAbstractListModel>
#include <QObject>
#include <QDateTime>

typedef struct _stage {
    QDateTime dateTime;
    QString title;
    QString details;
    QString result;
    int id;

    _stage(QDateTime d_date, QString s_title, QString s_details, QString s_result, int n_id = -1) {
        dateTime = d_date;
        title = s_title;
        details = s_details;
        result = s_result;
        id = n_id;
    }

    _stage() {
        title = "";
        details = "";
        result = "";
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
        IdRole
    };

    StageModel(QObject *parent = nullptr);
    ~StageModel();

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    virtual QHash<int, QByteArray> roleNames() const;

    void addStage(QDateTime date, QString title, QString details, QString result, int id);

    void removeStage(const int index);
    void removeStageWithId(const int id);

    void editDate(const int index,const QDateTime date);
    void editDate(const QDateTime date, const int id);

    void editTitle(const int index, const QString title);
    void editTitle(const QString title, const int id);

    void editDetails(const int index, const QString details);
    void editDetails(const QString details, const int id);

    void editResult(const int index, const QString result);
    void editResult(const QString result, const int id);

    void clear();

private:
    int IdSearchStage(const int id);

private:
    QVector<StageStruct> m_stageData;
};

#endif // STAGEMODEL_H
