#include "SubSchedule.h"

#include <QtDebug>

#define MAX_STAGE_NUMBER 10

SubSchedule::SubSchedule(QString name,QObject *parent)
    : QObject(parent),
      m_name(name)
{

}

SubSchedule::~SubSchedule()
{
    qDebug() << "destory:" << m_name;
}

bool SubSchedule::addStage(Stage* stage)
{
    if( m_stages.size() < MAX_STAGE_NUMBER ) {
        m_stages.append(stage);
        return true;
    }

    return false;
}

bool SubSchedule::removeStage(int index)
{
    if( index < m_stages.size() ) {
        Stage* stage = m_stages.takeAt(index);
        delete stage;
        stage = NULL;
        return true;
    }

    return false;
}

bool SubSchedule::editStage(int index, Stage* stage)
{
    if( index < m_stages.size() ) {
        m_stages.replace(index, stage);
        return true;
    }

    return false;
}

Stage* SubSchedule::selectStage(int index)
{
    if( index < m_stages.size() ) {
        return m_stages.at(index);
    }

    return new Stage();
}

void SubSchedule::clearStage()
{
    foreach(Stage* stage, m_stages)
    {
        delete stage;
        stage = NULL;
    }
    m_stages.clear();
}
