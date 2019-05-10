#include "Utils.h"

Utils::Utils(QObject *parent) : QObject(parent)
{

}

int Utils::checkStageWorkStatus(QDateTime stageTime, bool resultEmpty)
{
    QDateTime curDate = QDateTime::currentDateTime();

    if( stageTime >= curDate.addDays(3) )
        return status_waiting;

    else if ( stageTime >= curDate && stageTime < curDate.addDays(3) )
        return status_closeToExpire;

    else if ( curDate > stageTime && resultEmpty )
        return status_undone;

    else
        return status_done;
}
