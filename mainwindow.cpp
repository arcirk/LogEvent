#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include "dialogoptions.h"
#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //options = Settings();//this);
    //options->setServerLogFolder("/home/arcady/src/Log/LogEvent/data/srvinfo/reg_1541");
    options.get_server_info();

//    logDbPateh = "";
//    loadAppSettings();

//    ui->dtStaretDate->setDateTime(QDateTime::currentDateTime().addDays(-1));
//    ui->dtEndDate->setDateTime(QDateTime::currentDateTime());

//    period.startDate = ui->dtStaretDate->dateTime().currentSecsSinceEpoch();
//    period.endtDate = ui->dtEndDate->dateTime().currentSecsSinceEpoch();

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


void MainWindow::on_dtEndDate_dateTimeChanged(const QDateTime &dateTime)
{
   period.endtDate = dateTime.currentSecsSinceEpoch();
}


void MainWindow::on_dtStaretDate_dateTimeChanged(const QDateTime &dateTime)
{
    period.startDate = dateTime.currentSecsSinceEpoch();
}


void MainWindow::on_mnuExit_triggered()
{
    QApplication::exit();
}


void MainWindow::on_toolBtnUpdate_clicked()
{
    if(period.startDate > period.endtDate){
        QMessageBox::critical(this, "Ошибка", "Дата начала не может быть больше даты окончания!");
        return;
    }


}


void MainWindow::on_mnuOptions_triggered()
{
    auto dlg = new DialogOptions(this);
    dlg->setModal(true);
    dlg->exec();
    if(dlg->result() == QDialog::Accepted){
        loadAppSettings();
    }
}

void MainWindow::loadAppSettings()
{

//    QFile file("settings.json");
//    if (!file.open(QIODevice::ReadOnly))
//    {
//        return;
//    }

//    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
//    if(!doc.isNull()){
//        QJsonObject obj = doc.object();
//        auto _logDbPateh = obj.find("logDbPateh");
//        if(_logDbPateh->isString()){
//            logDbPateh = _logDbPateh->toString();
//        }
//        auto _mColVisible = obj.find("ColumnVisible");
//        if(_mColVisible->isArray()){
//            QJsonArray arr = _mColVisible->toArray();
//            for (auto iter : arr) {
//                QJsonObject item = iter.toObject();
//                bool val = item["value"].toBool();
//                selectedCols[item["name"].toString()] = val;
//            }

//        }
//    }
//    file.close();
}

