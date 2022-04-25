#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDateTime>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include "settings.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

struct standart_period{
    qint64 startDate = 0;
    qint64 endDate = 0;
};


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void connect_database();

private slots:
    void on_dtEndDate_dateTimeChanged(const QDateTime &dateTime);

    void on_dtStaretDate_dateTimeChanged(const QDateTime &dateTime);

    void on_mnuExit_triggered();

    void on_toolBtnUpdate_clicked();

    void on_mnuOptions_triggered();

    void on_mnuDbConnect_triggered();

    void on_mnuOpenSrvinfo_triggered();

    void on_mnuDbClose_triggered();

    void on_mnuColumnVisuble_triggered();

private:
    Ui::MainWindow *ui;
    standart_period period;
    QSqlDatabase dbLog;
    QSqlQueryModel * model;
    Settings  options;
    Infobases * currentIB;

};
#endif // MAINWINDOW_H
