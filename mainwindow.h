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
#include <QStandardItemModel>
#include <QStandardItem>
#include "querybuilderrunner.h"
#include "querybuilderthread.h"

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

    void on_btnConnectDb_clicked();

    void on_tableView_doubleClicked(const QModelIndex &index);

    void on_btnResetFilter_clicked();

    void on_btnSetFilterCurrentValue_clicked();

    void onUpdateFiltersOptions(const QString& uuid, const QString& name);
    void onUpdateAllFilterOptions(QList<filter_options*>& values);
    void onRemoveItemOptions(const QUuid& id);
    void onCopyItemOptions(const QUuid& source, const QUuid& result, const QString& name);
    void onLoadItemOptions(const QUuid& id);
    void on_btnCloseDb_clicked();
    void onSelectCurrentDay();
    void onSelectCurrentMonth();
    void onSelectStartOfYear();

    void on_chLimit_stateChanged(int arg1);

    void on_mnuAbout_triggered();

    void on_tableView_clicked(const QModelIndex &index);

    void onRunQueryfinished();

private:
    Ui::MainWindow *ui;
    standard_period period;
    QSqlDatabase dbLog;
    QSqlQueryModel * model;
    Settings  * options;
    Infobases * currentIB;
    QLabel * infoBar;
    FilterManager * filterManager;

    QAction* actionCurrentDay;
    QAction* actionCurrentMonth;
    QAction* actionStartOfYear;

    QMenu* pIntervalMenu;

    void setColumnsHidden();
    void read_filters_cache(const QUuid& uuid = QUuid{});
    void save_current_filter(const QString& uuid, const QString& newName);
    void connect_database();
    void close_database();
    void createActions();
    void createIntervalMenus();

    //QueryBuilderRunner * runner;

};
#endif // MAINWINDOW_H
