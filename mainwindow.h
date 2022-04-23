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
    uint startDate = 0;
    uint endtDate = 0;
};

enum bSettings{
    LogEventPath = 0,
    AppName
};

const QStringList ColumnNames ={"rowID",
                                "severity",
                                "date",
                                "connectID",
                                "session",
                                "transactionStatus",
                                "transactionDate",
                                "transactionID",
                                "userCode",
                                "computerCode",
                                "appCode",
                                "eventCode",
                                "comment",
                                "metadataCodes",
                                "sessionDataSplitCode",
                                "dataType",
                                "data",
                                "dataPresentation",
                                "workServerCode",
                                "primaryPortCode",
                                "secondaryPortCode"};

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

    void loadAppSettings();
    //void saveAppSettings();

private:
    Ui::MainWindow *ui;
    standart_period period;
    QString logDbPateh;
    QSqlDatabase dbLog;
    QSqlQueryModel * model;
    QMap<QString,bool> selectedCols;
    Settings * options;

};
#endif // MAINWINDOW_H
