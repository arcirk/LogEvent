#ifndef QUERYBUILDERRUNNER_H
#define QUERYBUILDERRUNNER_H

#include <QObject>
#include <QTime>
#include <QTimer>

class QueryBuilderRunner : public QObject
{
    Q_OBJECT
public:
    explicit QueryBuilderRunner(QObject *parent = nullptr);

protected:
    void changeEvent(QEvent *e);

public slots:
    void start();
    void stop();

    void updateTime();

private:
    QTime m_ElapsedTime;
    QTimer m_Timer;

signals:

};

#endif // QUERYBUILDERRUNNER_H
