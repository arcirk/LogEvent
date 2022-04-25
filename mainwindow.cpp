#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include "dialogoptions.h"
#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <dialogselectinfobase.h>
#include <QDir>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QFileDialog>
#include "dialogselectcolumn.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    options.getSettings();

    if(!options.v8srvinfo_catalog().isEmpty()){
        options.get_server_info(options.v8srvinfo_catalog());
        this->setWindowTitle("LogEvent:" + options.v8srvinfo_catalog());
    }

    currentIB = nullptr;

    dbLog = QSqlDatabase::addDatabase("QSQLITE");

//    logDbPateh = "";
//    loadAppSettings();

    ui->dtStaretDate->setDateTime(QDateTime::currentDateTime().addDays(-1));
    ui->dtEndDate->setDateTime(QDateTime::currentDateTime());

//    period.startDate = ui->dtStaretDate->dateTime().currentMSecsSinceEpoch();
//    period.endtDate = ui->dtEndDate->dateTime().currentMSecsSinceEpoch();

//    dbLog = QSqlDatabase::addDatabase("QSQLITE");
//    if(!logDbPateh.isEmpty()){
//        dbLog.setDatabaseName(logDbPateh);
//        dbLog.open();
//        if(!dbLog.isOpen()){
//            qDebug() << "cannot open database";
//        }else
//            qDebug() << "open database success";
//    }



//    model = new QSqlQueryModel(ui->tableView);

//    for (int i = 0; i < ColumnNames.size(); i++) {
//        selectedCols.insert(ColumnNames[i], false);
//    }



}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::connect_database()
{
    if(!currentIB){
        QMessageBox::critical(this, "Ошибка", "Информация о информационной базе не загружена!");
        return;
    }

    QString dbPath = options.v8srvinfo_logevent_catalog()
            + QDir::separator()
            + currentIB->GUID()
            + QDir::separator()
            + "1Cv8Log"
            + QDir::separator()
            + "1Cv8.lgd";

    dbPath = QDir::fromNativeSeparators(dbPath);

    qDebug() << dbPath;

    QFile dbFile(dbPath);
    if(!dbFile.exists()){
        QMessageBox::critical(this, "Ошибка", "Файл базы данных не найден!");
        return;
    }

    if(dbLog.isOpen())
        dbLog.close();

    dbLog.setDatabaseName(dbPath);
    dbLog.open();
    if(!dbLog.isOpen()){
        qDebug() << "cannot open database";
    }else
        qDebug() << "open database success";

}


void MainWindow::on_dtEndDate_dateTimeChanged(const QDateTime &dateTime)
{
   period.endDate = dateTime.toSecsSinceEpoch() + dateTime.offsetFromUtc();
   qDebug() << period.endDate;
}


void MainWindow::on_dtStaretDate_dateTimeChanged(const QDateTime &dateTime)
{
    period.startDate = dateTime.toSecsSinceEpoch() + dateTime.offsetFromUtc();
    //qDebug() << dateTime;
    qDebug() << period.startDate;
//    qDebug() << dateTime.toSecsSinceEpoch();
//    qDebug() << dateTime.offsetFromUtc();
}


void MainWindow::on_mnuExit_triggered()
{
    QApplication::exit();
}


void MainWindow::on_toolBtnUpdate_clicked()
{

    if(!dbLog.isOpen()){
        QMessageBox::critical(this, "Ошибка", "База данных не открыта!");
        return;
    }

    if(period.startDate > period.endDate){
        QMessageBox::critical(this, "Ошибка", "Дата начала не может быть больше даты окончания!");
        return;
    }


    QString query = "SELECT rowID,"
            "severity,"
            "date,"
            "connectID,"
            "session,"
            "transactionStatus,"
            "transactionDate,"
            "transactionID,"
            "userCode,"
            "computerCode,"
            "appCode,"
           "eventCode,"
            "comment,"
            "metadataCodes,"
            "sessionDataSplitCode,"
            "dataType,"
            "data,"
            "dataPresentation,"
            "workServerCode,"
            "primaryPortCode,"
            "secondaryPortCode"
       " FROM EventLog"
       " WHERE (date >= 637834570660002 AND date <= 637834570660005);";

    //QSqlQueryModel * model = new QSqlQueryModel(this, );
    //model->setQuery(query);
    //model->
//    ui->tableView->setModel(model);

    QSqlQuery _query(dbLog);
    _query.prepare(query);
    Q_ASSERT(_query.exec());

    QSqlTableModel * m_model = new QSqlTableModel(this, dbLog);
    m_model->setTable("EventLog");
    //(62135578800 + 1647878264) * 10000
    qint64 startDate = ((qint64)62135578800 + period.startDate) * (qint64)10000;
    qint64 endDate = ((qint64)62135578800 + period.endDate) * (qint64)10000;

    m_model->setFilter(QString("(date >= %1 AND date <= %2)").arg(QString::number(startDate), QString::number(endDate)));
    qDebug() << period.startDate;
    //m_model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    m_model->select();

    ui->tableView->setModel(m_model);

    qDebug() << QString::number(startDate) << "  " << QString::number(endDate);
}


void MainWindow::on_mnuOptions_triggered()
{
    auto dlg = new DialogOptions(this);
    dlg->setModal(true);
    dlg->exec();
    if(dlg->result() == QDialog::Accepted){
        //loadAppSettings();
    }
}


void MainWindow::on_mnuDbConnect_triggered()
{
    if(options.v8srvinfo_catalog().isEmpty()){
        QMessageBox::critical(this, "Ошибка", "Не найден каталог лог файлов!");
        return;
    }

    auto dlg = new DialogSelectInfobase(this, options.get_infobases());
    dlg->setModal(true);
    dlg->exec();
    if(dlg->result() == QDialog::Accepted){
        qDebug() << dlg->selectedItem;
        currentIB = options.get_infobases()[dlg->selectedItem];
        this->setWindowTitle("LogEvent:" + options.v8srvinfo_catalog() +  " (" + dlg->selectedItem + ")");
        connect_database();
    }
}


void MainWindow::on_mnuOpenSrvinfo_triggered()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Выбрать каталог"),
                                                 QDir::homePath(),
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks);
    if(dir != ""){
        if(dbLog.isOpen())
            dbLog.close();

        ui->tableView->setModel(nullptr);

        options.get_server_info(dir);
        this->setWindowTitle("LogEvent:" + options.v8srvinfo_catalog());
        options.saveSettings();
    }
}


void MainWindow::on_mnuDbClose_triggered()
{
    if(dbLog.isOpen()){
        dbLog.close();
        this->setWindowTitle(options.v8srvinfo_catalog());
    }
}


void MainWindow::on_mnuColumnVisuble_triggered()
{
    auto dlg = new DialogSelectColumn(this, options.get_selected_cols());
    dlg->setModal(true);
    dlg->exec();
    if(dlg->result() == QDialog::Accepted){

    }

}

