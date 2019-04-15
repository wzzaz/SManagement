#ifndef SUBSCHEDULE_H
#define SUBSCHEDULE_H

#include <QObject>
#include <QVector>
#include <QDateTime>

typedef struct _stageSrt{
    QDateTime date;
    QString details;
    QString result;

    _stageSrt(QDateTime d_date, QString s_details = "", QString s_result = "")
    {
        date = d_date;
        details = s_details;
        result = s_result;
    }

    _stageSrt() {}
} Stage;

class SubSchedule : public QObject
{
    Q_OBJECT
public:
    explicit SubSchedule(QString name, QObject *parent = nullptr);

    ~SubSchedule();

    SubSchedule() {}

    bool addStage(Stage* stage);

    bool removeStage(int index);

    bool editStage(int index, Stage* stage);

    Stage* selectStage(int index);

    void clearStage();

    void editName(QString name) { m_name = name; }

    QString name() { return m_name; }

    int stageSize() { return m_stages.size(); }

signals:

public slots:

private:

    QString m_name;

    QVector<Stage*> m_stages;
};

#endif // SUBSCHEDULE_H
