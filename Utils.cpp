#include "Utils.h"

#include <QDir>
#include <QDebug>

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

void Utils::createFile(QString filePath, QString fileName)
{
    QDir tempDir;
    QString currentDir = tempDir.currentPath();
    if(!tempDir.exists(filePath))
    {
        qDebug()<<QObject::tr("不存在该路径")<<endl;
        tempDir.mkpath(filePath);
    }
    QFile tempFile;
    tempDir.setCurrent(filePath);
    qDebug()<<tempDir.currentPath();
    if(QFile::exists(fileName))
    {
        qDebug()<<QObject::tr("文件存在");
        return ;
    }
    tempFile.setFileName(fileName);
    if(!tempFile.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        qDebug()<<QObject::tr("打开失败");
    }
    tempFile.close();
}
