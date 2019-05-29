#include "CommonManager.h"
#include "Utils.h"

CommonManager::CommonManager(QObject *parent) : QObject(parent)
{
    statusFilterInitialize();
}

void CommonManager::setStatusFilter(int status, bool checked)
{
    m_statusFilter.insert(status,checked);
    emit needRepaintView();
}

void CommonManager::statusFilterInitialize()
{
    m_statusFilter.clear();

    m_statusFilter.insert(status_done,true);
    m_statusFilter.insert(status_undone,true);
    m_statusFilter.insert(status_closeToExpire,true);
    m_statusFilter.insert(status_waiting,true);
}

QString CommonManager::statusFilterToSQLCode()
{
    QStringList filters;
    QMap<int,bool>::const_iterator i;
    for (i = m_statusFilter.constBegin(); i != m_statusFilter.constEnd(); ++i) {
        if ( i.value() )
            filters << QString::number(i.key());
    }

    return QString("(%1)").arg(filters.join(","));
}

bool CommonManager::isStatusChecked(int status)
{
    return m_statusFilter.value(status, false);
}
