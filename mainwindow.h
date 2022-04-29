#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDateTime>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include "settings.h"
#include <QSqlTableModel>
#include <querybuilder.h>
#include <QLabel>
#include "filtermanager.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

struct standard_period{
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

    void on_btnOpenFilterDlg_clicked();

private:
    Ui::MainWindow *ui;
    standard_period period;
    QSqlDatabase dbLog;
    QSqlQueryModel * model;
    //QSqlTableModel * m_model;
    Settings  * options;
    Infobases * currentIB;
    QLabel * infoBar;

    FilterManager * filterManager;

    void setColumnsHiden();

};
#endif // MAINWINDOW_H
