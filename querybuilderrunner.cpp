#include "querybuilderrunner.h"

QueryBuilderRunner::QueryBuilderRunner(QObject *parent)
    : QObject{parent}
{

}

void QueryBuilderRunner::start()
{

    connect(&m_Timer, SIGNAL(timeout()), this, SLOT(updateTime()));

    //m_ElapsedTime.restart();
    m_Timer.start();
}

void QueryBuilderRunner::stop()
{

}

void QueryBuilderRunner::updateTime()
{
    //QString elaspedTime = tr("Прошло: %1").arg(m_ElapsedTime.elapsed());
    qDebug() << "end query";

}
