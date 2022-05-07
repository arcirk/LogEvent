#ifndef QUERYBUILDERTHREAD_H
#define QUERYBUILDERTHREAD_H

#include <QThread>
#include "querybuilder.h"

class QueryBuilderThread : public QThread
{
    Q_OBJECT
public:
    explicit QueryBuilderThread(QueryBuilder *pModel,
                                QObject *parent = nullptr);

   void run() override;

signals:

public slots:

private:
    QueryBuilder *m_pModel;
    QString m_QueryString;

};

#endif // QUERYBUILDERTHREAD_H
