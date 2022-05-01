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
#include "dialogsqlfilter.h"
#include "dialogselectedrow.h"
#include "dialogsvalefilter.h"

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

    filterManager = new FilterManager();

    infoBar = new QLabel(this);
    ui->statusbar->addWidget(infoBar);
    infoBar->setText("Готово");

    connect(filterManager, &FilterManager::updateFilterOptions, this, &MainWindow::onUpdateFiltersOptions);
    connect(filterManager, &FilterManager::updateAllFilterOptions, this, &MainWindow::onUpdateAllFilterOptions);
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

    //qDebug() << dbPath;

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
    }else{
        qDebug() << "open database success";
        read_filters_cache();
    }

    filterManager->setDatabaseName(currentIB->Name());

}

void MainWindow::on_dtEndDate_dateTimeChanged(const QDateTime &dateTime)
{
    period.endDate = dateTime.toSecsSinceEpoch() + dateTime.offsetFromUtc();
    if(dbLog.isOpen()){
        on_toolBtnUpdate_clicked();
    }
}

void MainWindow::on_dtStaretDate_dateTimeChanged(const QDateTime &dateTime)
{
    period.startDate = dateTime.toSecsSinceEpoch() + dateTime.offsetFromUtc();
    if(dbLog.isOpen()){
        on_toolBtnUpdate_clicked();
    }
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

    QueryBuilder * mainModel = new QueryBuilder();
    mainModel->set_period(ui->dtStaretDate->dateTime(), ui->dtEndDate->dateTime());
    for (auto itr  : filterManager->filterItems()) {
        if(itr->use()){
            FilerData filerData;
            filerData.field = itr->key();
            filerData.type = itr->compareType();
            filerData.value = itr->value();
            mainModel->addFilter(filerData);
        }
    }

    QString err;
    mainModel->build(err);
    ui->tableView->setModel(mainModel);
    qDebug() << qPrintable(mainModel->toString());

    setColumnsHiden();
    //ui->tableView->resizeColumnsToContents();

    for (int i = 0; i < ui->tableView->model()->columnCount(); i++) {

        if(ui->tableView->model()->headerData(i, Qt::Horizontal, Qt::DisplayRole).toString() != "Комментарий")
            ui->tableView->resizeColumnToContents(i);

    }


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

void MainWindow::read_filters_cache()
{
    if(!currentIB)
        return;

    if(currentIB->Name().isEmpty())
        return;

   QFile file("filters.json");
   if (!file.open(QIODevice::ReadOnly))
   {
       return;
   }

   QJsonDocument doc = QJsonDocument::fromJson(file.readAll());

   file.close();

   if(!doc.isNull()){

       QJsonObject obj = doc.object();

       if(obj.isEmpty())
           return;

       auto itr = obj.find(currentIB->Name());

       if(itr == obj.end() || obj.isEmpty())
           return;

       auto _curDb = itr.value().toObject();

       for (auto itr = _curDb.begin(); itr != _curDb.end(); ++itr) {

           if(!itr.value().isObject())
               return;

           auto item = itr.value().toObject();

           auto f = item.find("loadCache");
           if(f != item.end()){
               bool loadCache = f.value().toBool();
               if(loadCache){
                   filterManager->setFiltersCache(item);
                   return;
               }
           }
       }

   }
}

void MainWindow::save_current_filter(const QString& uuid)
{
    if(filterManager->filterItems().size() == 0)
        return;

    QFile file("filters.json");
    if (!file.open(QIODevice::ReadWrite))
    {
        return;
    }

    QJsonObject objFilters = filterManager->toJsonObject();
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    QJsonObject obj = doc.object();
    QJsonObject opt;
    if(obj.isEmpty()){
        obj = QJsonObject();
    }else{
        auto itr = obj.find(currentIB->Name());

        if(itr != obj.end())
            opt = itr.value().toObject();
        else
            opt = QJsonObject();
    }

    opt.insert(uuid, objFilters);
    obj.insert(currentIB->Name(), opt);

    doc.setObject(obj);

    file.write(QJsonDocument(doc).toJson(QJsonDocument::Indented));

    file.close();

}


void MainWindow::on_btnOpenFilterDlg_clicked()
{
    if(!dbLog.isOpen()){
        QMessageBox::critical(this, "Ошибка", "База данных не открыта!");
        return;
    }
    auto dlg = new DialogSqlFilter(this, filterManager );
    dlg->setModal(true);
    dlg->exec();
    if(dlg->result() == QDialog::Accepted){
        on_toolBtnUpdate_clicked();
        if(filterManager->saveCache()){

        }
    }
}


void MainWindow::on_btnConnectDb_clicked()
{
    on_mnuDbConnect_triggered();
}


void MainWindow::on_tableView_doubleClicked(const QModelIndex &index)
{
    QMap<QString, QString> row;

    QModelIndex ind = ui->tableView->model()->index(index.row(), 0);
    row.insert("date", ind.data().toString());
    ind = ui->tableView->model()->index(index.row(), 1);
    row.insert("app", ind.data().toString());
    ind = ui->tableView->model()->index(index.row(), 2);
    row.insert("event", ind.data().toString());
    ind = ui->tableView->model()->index(index.row(), 3);
    row.insert("comp", ind.data().toString());
    ind = ui->tableView->model()->index(index.row(), 4);
    row.insert("comment", ind.data().toString());
    ind = ui->tableView->model()->index(index.row(), 5);
    row.insert("metadata", ind.data().toString());
    ind = ui->tableView->model()->index(index.row(), 6);
    row.insert("user", ind.data().toString());

    auto dlg = new DialogSelectedRow(row, this);
    dlg->exec();
    if(dlg->result() == QDialog::Accepted){

    }
}


void MainWindow::on_btnResetFilter_clicked()
{

    for (auto itr : filterManager->filterItems()) {
        itr->setUse(false);
    }

    on_toolBtnUpdate_clicked();
}


void MainWindow::on_btnSetFilterCurrentValue_clicked()
{
    if(!ui->tableView->currentIndex().isValid()){
        return;
    }

    QModelIndex index = ui->tableView->currentIndex();
    QString codeTable = options->get_code_table_from_alias_index(index.column());
    QString field = options->get_field_name_from_alias_index(index.column());
    QVariant value = index.data().toString();

    LogEventColumn colIndex = (LogEventColumn)ColumnNames.indexOf(field);

    if(codeTable.isEmpty()){
        filterManager->setFilter(colIndex, ComparisonType::equals, value, true, value);
        on_toolBtnUpdate_clicked();
    }else{
        QSqlQuery query(QString("select code from %1 where name = '%2';").arg(codeTable, value.toString()));
        QStringList values{};
        while (query.next()) {
            QString _value = query.value(0).toString();
            values.append(_value);
        }
        filterManager->setFilter(colIndex, ComparisonType::on_list, values, true, value);
        on_toolBtnUpdate_clicked();
    }

    ;}

void MainWindow::onUpdateFiltersOptions(const QString& uuid, const QString& name)
{

    filterManager->setNameOptions(name);
    filterManager->setDatabaseName(currentIB->Name());

    save_current_filter(uuid);

}

void MainWindow::onUpdateAllFilterOptions(QList<filter_options*>& values)
{

    QFile file("filters.json");
    if (!file.open(QIODevice::ReadWrite))
    {
        return;
    }

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());

    QJsonObject obj = doc.object();

    QJsonObject opt;

    bool find = false;

    if(obj.isEmpty()){
        obj = QJsonObject();
        opt = QJsonObject();
    }else{
        auto itr = obj.find(currentIB->Name());

        if(itr != obj.end())
            opt = itr.value().toObject();
        else
            opt = QJsonObject();

        find = false;
    }

    for (int i = 0; i < values.size(); ++i) {
        QJsonObject curr;
        if(opt.find(values.at(i)->uuid) != opt.end()){
            curr = opt[values.at(i)->uuid].toObject();
            curr["uuid"] = values.at(i)->uuid;
            curr["saveCache"] = values.at(i)->save;
            curr["nameOptions"] = values.at(i)->name;
            curr["loadCache"] = values.at(i)->load;
            opt[values.at(i)->uuid] = curr;
        }else{
            curr = QJsonObject();
            curr.insert("uuid", values.at(i)->uuid);
            curr.insert("saveCache", values.at(i)->save);
            curr.insert("nameOptions", values.at(i)->name);
            curr.insert("loadCache", values.at(i)->load);
            opt.insert(values.at(i)->uuid, curr);
        }

    }

    if(find)
        obj.insert(currentIB->Name(), opt);
    else
        obj[currentIB->Name()] = opt;

    doc = QJsonDocument();
    doc.setObject(obj);

    file.write(QJsonDocument(doc).toJson(QJsonDocument::Indented));

    file.close();

}

