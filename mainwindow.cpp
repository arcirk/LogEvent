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
#include <QFileDialog>
#include "dialogselectcolumn.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    options = new Settings(this);
    options->getSettings();

    if(!options->v8srvinfo_catalog().isEmpty()){
        options->get_server_info(options->v8srvinfo_catalog());
        this->setWindowTitle("LogEvent:" + options->v8srvinfo_catalog());
    }

    currentIB = nullptr;

    dbLog = QSqlDatabase::addDatabase("QSQLITE");

    ui->dtStaretDate->setDateTime(QDateTime::currentDateTime().addDays(-1));
    ui->dtEndDate->setDateTime(QDateTime::currentDateTime());

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

    QString dbPath = options->v8srvinfo_logevent_catalog()
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
   //qDebug() << period.endDate;
}


void MainWindow::on_dtStaretDate_dateTimeChanged(const QDateTime &dateTime)
{
    period.startDate = dateTime.toSecsSinceEpoch() + dateTime.offsetFromUtc();
    //qDebug() << period.startDate;
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


//    QString strquery = "SELECT rowID,"
//            "severity,"
//            "date,"
//            "connectID,"
//            "session,"
//            "transactionStatus,"
//            "transactionDate,"
//            "transactionID,"
//            "userCode,"
//            "computerCode,"
//            "appCode,"
//            "eventCode,"
//            "comment,"
//            "metadataCodes,"
//            "sessionDataSplitCode,"
//            "dataType,"
//            "data,"
//            "dataPresentation,"
//            "workServerCode,"
//            "primaryPortCode,"
//            "secondaryPortCode"
//       " FROM EventLog";
//
//    QSqlQueryModel * queryModel = new QSqlQueryModel();
//    queryModel->setQuery(strquery);
//
//    ui->tableView->setModel(queryModel);

    QueryBuilder * mainModel = new QueryBuilder();
    mainModel->set_period(ui->dtStaretDate->dateTime(), ui->dtEndDate->dateTime());
    FilerData filerData;
    filerData.field = "appCode";
    filerData.type = ComparisonType::equals;
    filerData.value = 0;
    mainModel->addFilter(filerData);
    QString err;
    mainModel->build(err);
    ui->tableView->setModel(mainModel);
    qDebug() << qPrintable(mainModel->toString());

    //ui->tableView->show();

    //model->setQuery(query);
    //model->
//    ui->tableView->setModel(model);

//    QSqlQuery _query(dbLog);
//    _query.prepare(query);
//    Q_ASSERT(_query.exec());

//    //
//    auto m_model = new QSqlTableModel(this, dbLog);
//    m_model->setTable("EventLog");
//    //(62135578800 + 1647878264) * 10000
//    qint64 startDate = ((qint64)62135578800 + period.startDate) * (qint64)10000;
//    qint64 endDate = ((qint64)62135578800 + period.endDate) * (qint64)10000;

//    m_model->setFilter(QString("(date >= %1 AND date <= %2)").arg(QString::number(startDate), QString::number(endDate)));
//    qDebug() << period.startDate;
//    //m_model->setEditStrategy(QSqlTableModel::OnManualSubmit);
//    m_model->select();
//    //

//    ui->tableView->setModel(m_model);

    setColumnsHiden();

    //qDebug() << QString::number(startDate) << "  " << QString::number(endDate);
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
    if(options->v8srvinfo_catalog().isEmpty()){
        QMessageBox::critical(this, "Ошибка", "Не найден каталог лог файлов!");
        return;
    }

    auto dlg = new DialogSelectInfobase(this, options->get_infobases());
    dlg->setModal(true);
    dlg->exec();
    if(dlg->result() == QDialog::Accepted){
        //qDebug() << dlg->selectedItem;
        currentIB = options->get_infobases()[dlg->selectedItem];
        this->setWindowTitle("LogEvent:" + options->v8srvinfo_catalog() +  " (" + dlg->selectedItem + ")");
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

        options->get_server_info(dir);
        this->setWindowTitle("LogEvent:" + options->v8srvinfo_catalog());
        options->saveSettings();
    }
}


void MainWindow::on_mnuDbClose_triggered()
{
    if(dbLog.isOpen()){
        dbLog.close();
        this->setWindowTitle(options->v8srvinfo_catalog());
        ui->tableView->setModel(nullptr);
    }else{
        QMessageBox::critical(this, "Ошибка", "База данных не открыта!");
    }
}


void MainWindow::on_mnuColumnVisuble_triggered()
{
    auto dlg = new DialogSelectColumn(this, options);
    dlg->setModal(true);
    dlg->exec();
    if(dlg->result() == QDialog::Accepted){
        options->saveSettings();
        setColumnsHiden();
    }

}

void MainWindow::setColumnsHiden()
{
    if(ui->tableView->model()){
        qDebug() << ui->tableView->model()->columnCount();
        for(int i = 0; i < ui->tableView->model()->columnCount(); i++)
        {
          QString col = ui->tableView->model()->headerData(i, Qt::Horizontal).toString();
          auto itr = options->get_selected_cols().find(col);
          if(itr != options->get_selected_cols().end())
              ui->tableView->setColumnHidden(i, !itr.value());
        }
        qDebug() << ui->tableView->model()->columnCount();
    }
}

