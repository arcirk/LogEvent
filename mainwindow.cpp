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
#include "dialogabout.h"
#include <QSortFilterProxyModel>
#include "filteritem.h"
#include <QDate>

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
    infoBar->setText("Не подключен.");

    connect(filterManager, &FilterManager::updateFilterOptions, this, &MainWindow::onUpdateFiltersOptions);
    connect(filterManager, &FilterManager::updateAllFilterOptions, this, &MainWindow::onUpdateAllFilterOptions);
    connect(filterManager, &FilterManager::removeItemOptions, this, &MainWindow::onRemoveItemOptions);
    connect(filterManager, &FilterManager::copyItemOptions, this, &MainWindow::onCopyItemOptions);
    connect(filterManager, &FilterManager::loadItemOptions, this, &MainWindow::onLoadItemOptions);

    ui->btnSetFilterCurrentValue->setCheckable(true);

    createActions();
    createIntervalMenus();

    ui->btnSelectInterval->setMenu(pIntervalMenu);
    //ui->btnSelectInterval->setDefaultAction(actionCurrentDay);

    //runner = new QueryBuilderRunner(this);
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

    QFile dbFile(dbPath);
    if(!dbFile.exists()){
        QMessageBox::critical(this, "Ошибка", "Файл базы данных не найден!");
        return;
    }

    if(dbLog.isOpen())
        close_database();

    dbLog.setDatabaseName(dbPath);
    dbLog.open();
    if(!dbLog.isOpen()){
        qDebug() << "cannot open database";
    }else{
        qDebug() << "open database success";
        read_filters_cache();
    }

    filterManager->setDatabaseName(currentIB->Name());

    infoBar->setText(QString("Подключен: %1").arg(currentIB->Name()));
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

    //QMessageBox::critical(this, "offsetFromUtc", QString::number(QDateTime::currentDateTime().offsetFromUtc()));

    if(!dbLog.isOpen()){
        QMessageBox::critical(this, "Ошибка", "База данных не открыта!");
        return;
    }

    if(period.startDate > period.endDate){
        QMessageBox::critical(this, "Ошибка", "Дата начала не может быть больше даты окончания!");
        return;
    }
//
//    QString stText = infoBar->text();
//    infoBar->setText("Чтение данных ...");

    auto * mainModel = new QueryBuilder(dbLog);

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

    if(ui->chLimit->checkState() == Qt::CheckState::Checked)
        mainModel->setLimit(ui->spinLimit->value());
    else
        mainModel->setLimit(-1);


    ui->tableView->setModel(0);



    mainModel->build();

    qDebug() << qPrintable(mainModel->toString());

//    QueryBuilderThread m_SqlThread = QueryBuilderThread(mainModel, this);
//    connect(&m_SqlThread, SIGNAL(finished()), this, SLOT(onRunQueryfinished));
//    m_SqlThread.start();

    QSortFilterProxyModel * proxyModel = new QSortFilterProxyModel();

    proxyModel->setSourceModel( mainModel );

    ui->tableView->setModel(proxyModel);

    //infoBar->setText(stText);

    ui->tableView->sortByColumn(0, Qt::AscendingOrder);

    setColumnsHidden();

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
           close_database();

        ui->tableView->setModel(nullptr);

        options->get_server_info(dir);
        this->setWindowTitle("LogEvent:" + options->v8srvinfo_catalog());
        options->saveSettings();
    }
}

void MainWindow::on_mnuDbClose_triggered()
{
    close_database();
}

void MainWindow::on_mnuColumnVisuble_triggered()
{
    auto dlg = new DialogSelectColumn(this, options);
    dlg->setModal(true);
    dlg->exec();
    if(dlg->result() == QDialog::Accepted){
        options->saveSettings();
        setColumnsHidden();
    }

}

void MainWindow::setColumnsHidden()
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

void MainWindow::read_filters_cache(const QUuid& uuid)
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

       if(uuid.isNull()){

           for (auto itr = _curDb.begin(); itr != _curDb.end(); ++itr) {

               if(!itr.value().isObject())
                   return;

               auto item = itr.value().toObject();

               auto f = item.find("loadCache");
               if(f != item.end()){
                   bool loadCache = f.value().toBool();
                   if(loadCache){
                       filterManager->setFiltersCache(item);
                   }
               }
           }
       }else{
           auto itrOpt = _curDb.find(uuid.toString());
           auto item = itrOpt.value().toObject();
           filterManager->setFiltersCache(item);
       }
   }
}

void MainWindow::save_current_filter(const QString& uuid, const QString& newName)
{

    QFile file("filters.json");
    bool exists = file.exists();
    if (!file.open(QIODevice::ReadOnly))
    {
        //return;
    }

    QJsonObject objFilters;
    objFilters = filterManager->toJsonObject();

    if(filterManager->uuid().toString() != uuid){
        objFilters["uuid"] = uuid;
        objFilters["nameOptions"] = newName;
    }

    QJsonDocument doc;
    if(exists){
        doc = QJsonDocument::fromJson(file.readAll());
        file.close();
    }else
        doc = QJsonDocument();


    QJsonObject obj = doc.object();
    QJsonObject opt;
    if(obj.isEmpty()){
        obj = QJsonObject();
        opt = QJsonObject();
    }else{
        auto itr = obj.find(currentIB->Name());

        if(itr != obj.end())
            opt = itr.value().toObject();
        else
            opt = QJsonObject();
    }

    opt.insert(uuid, objFilters);
    obj.insert(currentIB->Name(), opt);

    file.open(QIODevice::WriteOnly);

    doc.setObject(obj);

    file.write(QJsonDocument(doc).toJson(QJsonDocument::Indented));

    file.close();

}

void MainWindow::close_database()
{
    if(dbLog.isOpen()){
        dbLog.close();
        filterManager->reset();
        this->setWindowTitle(options->v8srvinfo_catalog());
        ui->tableView->setModel(nullptr);
        infoBar->setText("Не подключен.");

    }else{
        QMessageBox::critical(this, "Ошибка", "База данных не открыта!");
    }
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
        ui->btnSetFilterCurrentValue->setChecked(false);
        return;
    }

    if(ui->btnSetFilterCurrentValue->isChecked() ){ //сначала щелчек потом выделение

        QModelIndex index = ui->tableView->currentIndex();
        QString codeTable = options->get_code_table_from_alias_index(index.column());
        QString field = options->get_field_name_from_alias_index(index.column());
        QVariant value = index.data().toString();
        QList<FilterItem*> items = filterManager->findItemByName(field);

        if(items.size() > 0){
            for (auto item : items) {
                if(item->compareType() == ComparisonType::equals){
                    if(item->aliasesValue().toString() == value.toString()){
                        item->setUse(true);
                        on_toolBtnUpdate_clicked();
                        return;
                    }
                }
            }
        }

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

    }else{
        QString col = options->get_field_name_from_alias_index(ui->tableView->currentIndex().column());
        QList<FilterItem*> items = filterManager->findItemByName(col);
        for (auto item : items) {
            item->setUse(false);
            on_toolBtnUpdate_clicked();
        }
    }

}

void MainWindow::onUpdateFiltersOptions(const QString& uuid, const QString& name)
{

    save_current_filter(uuid, name);

}

void MainWindow::onUpdateAllFilterOptions(QList<filter_options*>& values)
{

    QFile file("filters.json");
    if (!file.open(QIODevice::ReadOnly))
    {
        return;
    }

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());

    file.close();

    QJsonObject objMain = doc.object();

    QJsonObject baseOptions;


    if (objMain.find(currentIB->Name()) != objMain.end())
        baseOptions = objMain[currentIB->Name()].toObject();
    else{
        baseOptions = QJsonObject();
        objMain.insert(currentIB->Name(), baseOptions);
    }

    qDebug() << filterManager->uuid().toString();

    for (int i = 0; i < values.size(); ++i) {
        QJsonObject currOption;

        if(baseOptions.find(values.at(i)->uuid) != baseOptions.end()){
            if(values.at(i)->uuid == filterManager->uuid().toString()){
                currOption = filterManager->toJsonObject();
                filterManager->setObjectName(currOption["nameOptions"].toString());
                filterManager->setSaveCache(currOption["saveCache"].toBool());
                filterManager->setloadCache(currOption["loadCache"].toBool());
            }else
                currOption = baseOptions[values.at(i)->uuid].toObject();

            currOption["saveCache"] = values.at(i)->save;
            currOption["nameOptions"] = values.at(i)->name;
            currOption["loadCache"] = values.at(i)->load;

            baseOptions[values.at(i)->uuid] = currOption;
        }else{
            currOption = QJsonObject();
            currOption.insert("databaseName", currentIB->Name());
            QJsonArray arr = QJsonArray();
            currOption.insert("filterItems", arr);
            currOption.insert("loadCache", values.at(i)->load);
            currOption.insert("nameOptions", values.at(i)->name);
            currOption.insert("saveCache", values.at(i)->save);
            currOption.insert("uuid", values.at(i)->uuid);
            baseOptions.insert(values.at(i)->uuid, currOption);
        }
    }

    objMain[currentIB->Name()] = baseOptions;

    file.open(QIODevice::WriteOnly);
    file.write(QJsonDocument(objMain).toJson(QJsonDocument::Indented));

    file.close();

}

void MainWindow::onRemoveItemOptions(const QUuid &id)
{
    QFile file("filters.json");
    if (!file.open(QIODevice::ReadOnly))
    {
        return;
    }

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());

    file.close();

    QJsonObject objMain = doc.object();

    QJsonObject baseOptions;


    if (objMain.find(currentIB->Name()) != objMain.end())
        baseOptions = objMain[currentIB->Name()].toObject();
    else{
        baseOptions = QJsonObject();
        objMain.insert(currentIB->Name(), baseOptions);
    }

    auto itr = baseOptions.find(id.toString());
    if(itr != baseOptions.end())
        baseOptions.remove(id.toString());


    objMain[currentIB->Name()] = baseOptions;

    file.open(QIODevice::WriteOnly);
    file.write(QJsonDocument(objMain).toJson(QJsonDocument::Indented));

    file.close();

}

void MainWindow::onCopyItemOptions(const QUuid &source, const QUuid &result, const QString& name)
{
    QFile file("filters.json");
    if (!file.open(QIODevice::ReadOnly))
    {
        return;
    }

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());

    file.close();

    QJsonObject objMain = doc.object();

    QJsonObject baseOptions;


    if (objMain.find(currentIB->Name()) != objMain.end())
        baseOptions = objMain[currentIB->Name()].toObject();
    else{
        baseOptions = QJsonObject();
        objMain.insert(currentIB->Name(), baseOptions);
    }

    auto itr = baseOptions.find(source.toString());



    if(itr != baseOptions.end()){
        QJsonObject newObj = itr.value().toObject();
        newObj["uuid"] = result.toString();
        newObj["nameOptions"] = name;
        baseOptions.insert(result.toString(), newObj);

    }

    objMain[currentIB->Name()] = baseOptions;

    file.open(QIODevice::WriteOnly);
    file.write(QJsonDocument(objMain).toJson(QJsonDocument::Indented));

    file.close();
}

void MainWindow::onLoadItemOptions(const QUuid &id)
{
    read_filters_cache(id);
    emit filterManager->resetFilter();
}


void MainWindow::on_btnCloseDb_clicked()
{
    close_database();
}

void MainWindow::onSelectCurrentDay()
{
    QDate dt = QDate::currentDate();
    ui->dtStaretDate->setDateTime(dt.startOfDay());
    ui->dtEndDate->setDateTime(dt.endOfDay());
}


void MainWindow::on_chLimit_stateChanged(int arg1)
{
    ui->spinLimit->setEnabled(arg1);
}


void MainWindow::on_mnuAbout_triggered()
{
    auto dlg = new DialogAbout(this);
    dlg->setModal(true);
    dlg->exec();
}


void MainWindow::on_tableView_clicked(const QModelIndex &index)
{
    QString col = options->get_field_name_from_alias_index(index.column());

    QList<FilterItem*> items = filterManager->findItemByName(col);
    bool isUse = false;
    if(items.size() > 0){
        for (auto item : items) {
            if(item->use()){
                isUse = true;
                break;
            }
        }
    }

    ui->btnSetFilterCurrentValue->setChecked(isUse);
}

void MainWindow::onRunQueryfinished()
{
    qDebug() << "onRunQueryfinished";
}

void MainWindow::createActions(){

    actionCurrentDay = new QAction("Текущий день", this);
    actionCurrentMonth = new QAction("Текущий месяц", this);
    actionStartOfYear = new QAction("С начала текущего года", this);

    //alignLeftAction->setIcon(QIcon(":/icons/alignLeft.png"));

    QObject::connect(actionCurrentDay, SIGNAL(triggered()),
                                this, SLOT(onSelectCurrentDay()));
    QObject::connect(actionCurrentMonth, SIGNAL(triggered()),
                     this, SLOT(onSelectCurrentMonth()));
    QObject::connect(actionStartOfYear, SIGNAL(triggered()),
                     this, SLOT(onSelectStartOfYear()));
}

void MainWindow::createIntervalMenus(){
    pIntervalMenu = new QMenu;
    pIntervalMenu->addAction(actionCurrentDay);
    pIntervalMenu->addAction(actionCurrentMonth);
    pIntervalMenu->addAction(actionStartOfYear);
}

void MainWindow::onSelectCurrentMonth() {
    QDate dt = QDate::currentDate();
    QDate startMonth = QDate(dt.year(), dt.month(), 1);
    QDate endMonth = QDate(dt.year(), dt.month(),  dt.daysInMonth());
    ui->dtStaretDate->setDateTime(startMonth.startOfDay());
    ui->dtEndDate->setDateTime(endMonth.endOfDay());
}

void MainWindow::onSelectStartOfYear() {
    QDate dt = QDate::currentDate();
    QDate startMonth = QDate(dt.year(), 1, 1);
    ui->dtStaretDate->setDateTime(startMonth.startOfDay());
    ui->dtEndDate->setDateTime(QDateTime::currentDateTime());
}
