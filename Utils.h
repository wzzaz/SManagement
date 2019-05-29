#ifndef UTILS_H
#define UTILS_H

#include <QObject>
#include <QDateTime>

enum
{
    status_done = 1,
    status_undone,
    status_closeToExpire,
    status_waiting
};

class Utils : public QObject
{
    Q_OBJECT
public:
    explicit Utils(QObject *parent = nullptr);

    static int checkStageWorkStatus(QDateTime stageTime, bool resultEmpty);

    static void createFile(QString filePath, QString fileName);

};

#endif // UTILS_H
