#ifndef COMMONMANAGER_H
#define COMMONMANAGER_H

#include <QMap>
#include <QObject>

class CommonManager : public QObject
{
    Q_OBJECT
public:
    explicit CommonManager(QObject *parent = nullptr);

    Q_INVOKABLE void setStatusFilter(int status, bool checked);

    void statusFilterInitialize();

    QString statusFilterToSQLCode();

    bool isStatusChecked(int status);

signals:
    void needRepaintView();

public slots:

private:
    QMap<int, bool> m_statusFilter;
};

#endif // COMMONMANAGER_H
