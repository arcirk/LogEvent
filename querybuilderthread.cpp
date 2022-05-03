#include "querybuilderthread.h"


QueryBuilderThread::QueryBuilderThread(QueryBuilder *pModel, QObject *parent) :
  QThread(parent)
{
    m_pModel = pModel;
   // m_QueryString = queryString;

    m_pModel->moveToThread(this);
}

void QueryBuilderThread::run()
{

    m_pModel->build();
    //m_pModel->setQuery(m_QueryString);
}
