#include "dialogsavefilter.h"
#include "ui_dialogsavefilter.h"
#include <QUuid>
#include <QCheckBox>
#include <QList>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>

DialogSaveFilter::DialogSaveFilter(FilterManager * manager, bool SelectedMode, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSaveFilter)
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

    currentRowLoad = -1;

    init_model();

    set_default_item();
}

DialogSaveFilter::~DialogSaveFilter()
{
    delete ui;
}

void DialogSaveFilter::accept()
{

    if(!_selectedMode){
        QList<filter_options*> values;

        for (int i = 0; i < ui->tableWidget->rowCount(); i++) {
            QString name = ui->tableWidget->item(i, 0)->text();
            QString uuid = ui->tableWidget->item(i, 3)->text();
            int keyString = ui->tableWidget->item(i, 4)->text().toInt();

            bool save = false;
            bool load = false;

            QWidget* pWidget = ui->tableWidget->cellWidget(i, 1);
            if(pWidget){
                QCheckBox * checkBox = pWidget->findChild<QCheckBox*>("save" + QString::number(keyString));

                if(checkBox && checkBox->checkState() == Qt::CheckState::Checked){
                    save = true;
                }
            }

            QWidget* pWidget1 = ui->tableWidget->cellWidget(i, 2);
            if(pWidget1){
                QCheckBox * checkBox1 = pWidget1->findChild<QCheckBox*>("load" + QString::number(keyString));

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
    }else{

        int row = ui->tableWidget->currentRow();
        if(row < 0)
        {
            QMessageBox::critical(this, "Ошибка", "Не выбран элемент!");
            return;
        }

    }

    QDialog::accept();
}

QUuid DialogSaveFilter::getResult()
{
    return _result;
}

void DialogSaveFilter::on_btnAdd_clicked()
{
    add_filter_item();
    set_default_item();
}

void DialogSaveFilter::init_model()
{

    QTableWidget* table = ui->tableWidget;

    table->setColumnCount(5);
    table->setHorizontalHeaderItem(0, new QTableWidgetItem("Наименование"));
    table->setHorizontalHeaderItem(1, new QTableWidgetItem("Сохранять"));
    table->setHorizontalHeaderItem(2, new QTableWidgetItem("Востанавливать"));
    table->setHorizontalHeaderItem(3, new QTableWidgetItem("Идентификатор"));
    table->setHorizontalHeaderItem(4, new QTableWidgetItem("КлючСтроки"));
    table->setColumnHidden(3, true);
    table->setColumnHidden(4, true);
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

    bool def = false;
    int i = 0;
    for (auto itr = opt.begin(); itr != opt.end(); ++itr) {
        QJsonObject curr = itr.value().toObject();
        auto * _opt = new filter_options();

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


        if(_opt->load && !def)
        {
            def = true;
            _opt->def = true;
            _opt->load = true;
            currentRowLoad = i;
        }else{
            _opt->load = false;
        }

        add_filter_item(_opt);

        i++;
    }

    qDebug() << currentRowLoad;
}

void DialogSaveFilter::add_filter_item(filter_options *_opt)
{
    QTableWidget* table = ui->tableWidget;

    table->setRowCount(table->rowCount() + 1);

    int currentRow = table->rowCount() - 1;

    if(currentRowLoad < 0 && currentRow == 0 && !_opt)
        currentRowLoad = currentRow;

    auto *pItemKey = new QTableWidgetItem(QString::number(currentRow));
    pItemKey->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
    table->setItem(currentRow,4,pItemKey);

    QString name = QString("Новая настройка %1").arg(QString::number(currentRow+1));
    if(_opt)
        name = _opt->name;

    auto *pItem = new QTableWidgetItem(name);
    if(_selectedMode)
        pItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
    table->setItem(table->rowCount()-1,0,pItem);

    QString uuid = QUuid::createUuid().toString();
    if(_opt)
        uuid = _opt->uuid;
    auto *pItemUuid = new QTableWidgetItem(uuid);
    table->setItem(table->rowCount()-1,3,pItemUuid);

    auto *pWidget = new QWidget();
    auto *pCheckBox = new QCheckBox();
    auto *pLayout = new QHBoxLayout(pWidget);
    pLayout->addWidget(pCheckBox);
    pLayout->setAlignment(Qt::AlignCenter);
    pLayout->setContentsMargins(0,0,0,0);
    pWidget->setLayout(pLayout);
    table->setCellWidget(table->rowCount()-1,1,pWidget);
    pCheckBox->setCheckState(Qt::CheckState::Unchecked);
    if(_opt){
        if(_opt->save)
           pCheckBox->setCheckState(Qt::CheckState::Checked);
    }
    pCheckBox->setObjectName("save" + QString::number(currentRow));
    pCheckBox->setEnabled(!_selectedMode);

    auto *pWidget1 = new QWidget();
    auto *pCheckBox1 = new QCheckBox();
    auto *pLayout1 = new QHBoxLayout(pWidget1);
    pLayout1->addWidget(pCheckBox1);
    pLayout1->setAlignment(Qt::AlignCenter);
    pLayout1->setContentsMargins(0,0,0,0);
    pWidget1->setLayout(pLayout1);
    table->setCellWidget(table->rowCount()-1,2,pWidget1);
    pCheckBox1->setCheckState(Qt::CheckState::Unchecked);
    pCheckBox1->setEnabled(!_selectedMode);
    if(_opt){
        if(_opt->load)
           pCheckBox1->setCheckState(Qt::CheckState::Checked);
    }else{
        if(currentRow == 0 && !_selectedMode){
            pCheckBox1->setCheckState(Qt::CheckState::Checked);
        }
    }
    pCheckBox1->setObjectName("load" + QString::number(currentRow));
    table->resizeColumnsToContents();

    if(!_selectedMode){
        if(!_opt)
            emit _manager->updateFilterOptions(uuid, name);
    }

    if(_opt)
        delete _opt;

    connect(pCheckBox1, SIGNAL(toggled(bool)), this, SLOT(onCheckBoxToggled(bool)));
}

void DialogSaveFilter::set_default_item()
{
    QFont font;

    for (int i = 0; i < ui->tableWidget->rowCount(); i++) {
        int keyString =  ui->tableWidget->item(i, 4)->text().toInt();
        QWidget* pWidget = ui->tableWidget->cellWidget(i, 2);
        auto * checkBox = pWidget->findChild<QCheckBox*>("load" + QString::number(keyString));
        if(checkBox->checkState() == Qt::CheckState::Checked)
        {
            font.setBold(true);
        }else
            font.setBold(false);

        ui->tableWidget->item(i, 0)->setFont(font);
    }
}


void DialogSaveFilter::on_btnDelete_clicked()
{
    if(ui->tableWidget->currentRow() < 0)
        return;

    QTableWidgetItem * item = ui->tableWidget->item(ui->tableWidget->currentRow(), 3);
    emit _manager->removeItemOptions(QUuid::fromString(item->text()));

    ui->tableWidget->removeRow(ui->tableWidget->currentRow());
}


void DialogSaveFilter::on_btnCopy_clicked()
{
    if(ui->tableWidget->currentRow() < 0){
         QMessageBox::critical(this, "Ошибка", "Не выбран элемент!");
    }

    int row = ui->tableWidget->currentRow();

    QTableWidgetItem * item = ui->tableWidget->item(row, 0);

    int keyString =  ui->tableWidget->item(row, 4)->text().toInt();

    auto *_opt = new filter_options();
    QString name = item->text() + " 1";
    _opt->name = name;

    QWidget* pWidget = ui->tableWidget->cellWidget(row, 1);
    auto * checkBox = pWidget->findChild<QCheckBox*>("save" + QString::number(keyString));
    _opt->save = false;

    pWidget = ui->tableWidget->cellWidget(row, 2);
    checkBox = pWidget->findChild<QCheckBox*>("load" + QString::number(keyString));
     _opt->load = false;

    QUuid result = QUuid::createUuid();

    item = ui->tableWidget->item(row, 3);
    QUuid source = QUuid::fromString(item->text());

    _opt->uuid = result.toString();

    add_filter_item(_opt);

    emit _manager->copyItemOptions(source, result, name);

}


void DialogSaveFilter::on_btnSelect_clicked()
{
    if(!_selectedMode)
        return;

    if(ui->tableWidget->currentRow() < 0){
         QMessageBox::critical(this, "Ошибка", "Не выбран элемент!");
    }
    int row = ui->tableWidget->currentRow();

    auto item = ui->tableWidget->item(row, 3);

    _result = QUuid::fromString(item->text());

    accept();
}


void DialogSaveFilter::on_tableWidget_doubleClicked(const QModelIndex &index)
{
    on_btnSelect_clicked();
}

void DialogSaveFilter::onCheckBoxToggled(bool value)
{
    if(value){
        if(currentRowLoad < 0)
        {
            for (int i = 0; i < ui->tableWidget->rowCount(); i++) {
                int keyString =  ui->tableWidget->item(i, 4)->text().toInt();
                QWidget* pWidget = ui->tableWidget->cellWidget(i, 2);
                auto * checkBox = pWidget->findChild<QCheckBox*>("load" + QString::number(keyString));
                if(checkBox->checkState() == Qt::CheckState::Checked)
                {
                    currentRowLoad = i;
                    break;;
                }
            }
        }else{
            int selected = -1;
            for (int i = 0; i < ui->tableWidget->rowCount(); i++) {
                int keyString =  ui->tableWidget->item(i, 4)->text().toInt();
                QWidget* pWidget = ui->tableWidget->cellWidget(i, 2);
                auto * checkBox = pWidget->findChild<QCheckBox*>("load" + QString::number(keyString));
                if(checkBox->checkState() == Qt::CheckState::Checked)
                {
                    if(i == currentRowLoad){
                        checkBox->setCheckState(Qt::CheckState::Unchecked);
                    }else
                        if(selected < 0)
                            selected = i;

                    if(selected != -1 && i != selected){
                        checkBox->setCheckState(Qt::CheckState::Unchecked);
                    }
                }

            }
            currentRowLoad = selected;
        }

    }

    if(currentRowLoad != -1){
        ui->tableWidget->setCurrentItem(ui->tableWidget->item(currentRowLoad, 0));
    }
    set_default_item();
}

void DialogSaveFilter::on_buttonBox_accepted()
{

}

