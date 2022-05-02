#include "dialogsvalefilter.h"
#include "ui_dialogsvalefilter.h"
#include <QUuid>
#include <QCheckBox>
#include <QList>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>

DialogSvaleFilter::DialogSvaleFilter(FilterManager * manager, bool SelectedMode, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSvaleFilter)
{
    ui->setupUi(this);
    if(!SelectedMode){
        ui->btnSelect->setVisible(false);
        setWindowTitle("Настройки отборов");
    }
    else{
        ui->btnAdd->setVisible(false);
        ui->btnCopy->setVisible(false);
        ui->btnDelete->setVisible(false);
        ui->buttonBox->setVisible(false);
        setWindowTitle("Настройки отборов (Выбор)");
    }

    _selectedMode = SelectedMode;
    _manager = manager;

    init_model();

}

DialogSvaleFilter::~DialogSvaleFilter()
{
    delete ui;
}

void DialogSvaleFilter::accept()
{

    if(!_selectedMode){
        QList<filter_options*> values;
        for (int i = 0; i < ui->tableWidget->rowCount(); i++) {
            QString name = ui->tableWidget->item(i, 0)->text();
            QString uuid = ui->tableWidget->item(i, 3)->text();
            bool save = false;
            bool load = false;

            QWidget* pWidget = ui->tableWidget->cellWidget(i, 1);
            if(pWidget){
                QCheckBox * checkBox = pWidget->findChild<QCheckBox*>("save" + QString::number(i));

                if(checkBox && checkBox->checkState() == Qt::CheckState::Checked){
                    save = true;
                }
            }

            QWidget* pWidget1 = ui->tableWidget->cellWidget(i, 2);
            if(pWidget1){
                QCheckBox * checkBox1 = pWidget1->findChild<QCheckBox*>("load" + QString::number(i));

                if(checkBox1 && checkBox1->checkState() == Qt::CheckState::Checked){
                    load = true;
                }
            }
            auto fo = new filter_options();
            fo->load = load;
            fo->save = save;
            fo->name = name;
            fo->uuid = uuid;

            values.append(fo);
            if(uuid == _manager->uuid().toString())
                _manager->setNameOptions(name);
        }
        emit _manager->updateAllFilterOptions(values);
    }

    QDialog::accept();
}

QUuid DialogSvaleFilter::getResult()
{
    return _result;
}

void DialogSvaleFilter::on_btnAdd_clicked()
{
    add_filter_item();
}

void DialogSvaleFilter::init_model()
{

    QTableWidget* table = ui->tableWidget;

    table->setColumnCount(4);
    table->setHorizontalHeaderItem(0, new QTableWidgetItem("Наименование"));
    table->setHorizontalHeaderItem(1, new QTableWidgetItem("Сохранять"));
    table->setHorizontalHeaderItem(2, new QTableWidgetItem("Востанавливать"));
    table->setHorizontalHeaderItem(3, new QTableWidgetItem("Идентификатор"));
    table->setColumnHidden(3, true);
    table->resizeColumnsToContents();

    ui->horizontalLayout->addStretch();

    QFile file("filters.json");
    if (!file.open(QIODevice::ReadOnly))
    {
        return;
    }

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());

    file.close();

    QJsonObject obj = doc.object();
    QJsonObject opt;

    if(obj.isEmpty()){
        return;
    }else{
        auto itr = obj.find(_manager->databaseName());

        if(itr != obj.end())
            opt = itr.value().toObject();
        else
            return;
    }

    for (auto itr = opt.begin(); itr != opt.end(); ++itr) {
        QJsonObject curr = itr.value().toObject();
        filter_options * _opt = new filter_options();

        auto item = curr.find("nameOptions");
        if(item != curr.end())
            _opt->name = item.value().toString();
        else
            continue;

        item = curr.find("uuid");

        if(item != curr.end())
            _opt->uuid = item.value().toString();
        else
            _opt->uuid = QUuid::createUuid().toString();

        item = curr.find("saveCache");
        if(item != curr.end())
            _opt->save = item.value().toBool();
        else
            _opt->save = false;

        item = curr.find("loadCache");
        if(item != curr.end())
            _opt->load = item.value().toBool();
        else
            _opt->load = false;

        add_filter_item(_opt);

    }
}

void DialogSvaleFilter::add_filter_item(filter_options *_opt)
{
    QTableWidget* table = ui->tableWidget;
    table->setRowCount(table->rowCount() + 1);

    QString name = "Новая настройка";
    if(_opt)
        name = _opt->name;
    QTableWidgetItem *pItem = new QTableWidgetItem(name);
    table->setItem(table->rowCount()-1,0,pItem);
    QString uuid = QUuid::createUuid().toString();
    if(_opt)
        uuid = _opt->uuid;
    QTableWidgetItem *pItemUuid = new QTableWidgetItem(uuid);
    table->setItem(table->rowCount()-1,3,pItemUuid);

    QWidget *pWidget = new QWidget();
    QCheckBox *pCheckBox = new QCheckBox();
    QHBoxLayout *pLayout = new QHBoxLayout(pWidget);
    pLayout->addWidget(pCheckBox);
    pLayout->setAlignment(Qt::AlignCenter);
    pLayout->setContentsMargins(0,0,0,0);
    pWidget->setLayout(pLayout);
    table->setCellWidget(table->rowCount()-1,1,pWidget);
    pCheckBox->setCheckState(Qt::CheckState::Checked);
    if(_opt){
        if(!_opt->save)
           pCheckBox->setCheckState(Qt::CheckState::Unchecked);
    }
    pCheckBox->setObjectName("save" + QString::number(table->rowCount()-1));

    QWidget *pWidget1 = new QWidget();
    QCheckBox *pCheckBox1 = new QCheckBox();
    QHBoxLayout *pLayout1 = new QHBoxLayout(pWidget1);
    pLayout1->addWidget(pCheckBox1);
    pLayout1->setAlignment(Qt::AlignCenter);
    pLayout1->setContentsMargins(0,0,0,0);
    pWidget1->setLayout(pLayout1);
    table->setCellWidget(table->rowCount()-1,2,pWidget1);
    pCheckBox1->setCheckState(Qt::CheckState::Checked);
    if(_opt){
        if(!_opt->load)
           pCheckBox1->setCheckState(Qt::CheckState::Unchecked);
    }
    pCheckBox1->setObjectName("load" + QString::number(table->rowCount()-1));
    table->resizeColumnsToContents();

    if(!_selectedMode){
        if(!_opt)
            emit _manager->updateFilterOptions(uuid, "Новая настройка");
    }

    if(_opt)
        delete _opt;
}


void DialogSvaleFilter::on_btnDelete_clicked()
{
    if(ui->tableWidget->currentRow() < 0)
        return;

    QTableWidgetItem * item = ui->tableWidget->item(ui->tableWidget->currentRow(), 3);
    emit _manager->removeItemOptions(QUuid::fromString(item->text()));

    ui->tableWidget->removeRow(ui->tableWidget->currentRow());
}


void DialogSvaleFilter::on_btnCopy_clicked()
{
    if(ui->tableWidget->currentRow() < 0){
         QMessageBox::critical(this, "Ошибка", "Не выбран элемент!");
    }

    int row = ui->tableWidget->currentRow();

    QTableWidgetItem * item = ui->tableWidget->item(row, 0);

    filter_options *_opt = new filter_options();
    QString name = item->text() + " 1";
    _opt->name = name;

    QWidget* pWidget = ui->tableWidget->cellWidget(row, 1);
    QCheckBox * checkBox = pWidget->findChild<QCheckBox*>("save" + QString::number(row));
    if(checkBox){
        if(checkBox->checkState() == Qt::CheckState::Checked)
            _opt->save = true;
        else
            _opt->save = false;
    }
    pWidget = ui->tableWidget->cellWidget(row, 2);
    checkBox = pWidget->findChild<QCheckBox*>("load" + QString::number(row));
    if(checkBox){
        if(checkBox->checkState() == Qt::CheckState::Checked)
            _opt->load = true;
        else
            _opt->load = false;
    }

    QUuid result = QUuid::createUuid();

    item = ui->tableWidget->item(row, 3);
    QUuid source = QUuid::fromString(item->text());

    _opt->uuid = result.toString();

    add_filter_item(_opt);

    emit _manager->copyItemOptions(source, result, name);

}


void DialogSvaleFilter::on_btnSelect_clicked()
{
    if(ui->tableWidget->currentRow() < 0){
         QMessageBox::critical(this, "Ошибка", "Не выбран элемент!");
    }
    int row = ui->tableWidget->currentRow();

    auto item = ui->tableWidget->item(row, 3);
    _result = QUuid::fromString(item->text());

    accept();
}
