#include "dialogsqlfilter.h"
#include "ui_dialogsqlfilter.h"
#include <QTableWidget>
#include "dialogselectitemsfilter.h"
#include "settings.h"
#include <selectiondialog.h>
#include <QComboBox>
#include <QStringListModel>
#include <QLineEdit>
#include <QToolButton>
#include <QPushButton>
#include <QDateTimeEdit>
#include <dialogsavefilter.h>

DialogSqlFilter::DialogSqlFilter(QWidget *parent, FilterManager * filterManager) :
    QDialog(parent),
    ui(new Ui::DialogSqlFilter)
{
    ui->setupUi(this);

    _filterManager = filterManager;

    QTableWidget* table = ui->tableWidget;
    table->setColumnCount(6);
    table->setHorizontalHeaderItem(0, new QTableWidgetItem(""));
    table->setHorizontalHeaderItem(1, new QTableWidgetItem("Поле"));
    table->setHorizontalHeaderItem(2, new QTableWidgetItem("Вид сравнения"));
    table->setHorizontalHeaderItem(3, new QTableWidgetItem("Значение"));
    table->setHorizontalHeaderItem(4, new QTableWidgetItem("Идентификатор"));
    table->setHorizontalHeaderItem(5, new QTableWidgetItem("КлючСтроки"));
    table->setColumnHidden(4, true);
    table->setColumnHidden(5, true);
    table->resizeColumnsToContents();

    ui->toolBarFilter->addStretch();

    _compareType.insert("равно", ComparisonType::equals);
    _compareType.insert("не равно", ComparisonType::no_equals);
    _compareType.insert("больше", ComparisonType::more);
    _compareType.insert("больше или равно", ComparisonType::more_or_equal);
    _compareType.insert("меньше", ComparisonType::less);
    _compareType.insert("меньше или равно", ComparisonType::less);
    _compareType.insert("в списке", ComparisonType::on_list);
    _compareType.insert("не в списке", ComparisonType::not_in_list);

    _aliases.insert("Приложение", "AppCodes");
    _aliases.insert("Событие", "EventCodes");
    _aliases.insert("Компьютер", "ComputerCodes");
    _aliases.insert("Метаданные", "MetadataCodes");
    _aliases.insert("Пользователь", "UserCodes");

    _aliasesField.insert("Приложение", "appCode");
    _aliasesField.insert("Событие", "eventCode");
    _aliasesField.insert("Компьютер", "computerCode");
    _aliasesField.insert("Метаданные", "metadataCodes");
    _aliasesField.insert("Пользователь", "userCode");
    _aliasesField.insert("Комментарий", "comment");
    _aliasesField.insert("Дата", "date");
    _aliasesField.insert("date", "Дата");
    _aliasesField.insert("comment", "Комментарий");
    _aliasesField.insert("appCode", "Приложение");
    _aliasesField.insert("eventCode", "Событие");
    _aliasesField.insert("computerCode", "Компьютер");
    _aliasesField.insert("metadataCodes", "Метаданные");
    _aliasesField.insert("userCode", "Пользователь");
    _aliasesField.insert("rowID", "rowID");
    _aliasesField.insert("dataPresentation", "dataPresentation");
    _aliasesField.insert("data", "data");
    _aliasesField.insert("dataType", "dataType");

    connect(this, &DialogSqlFilter::selectedFormShow, this, &DialogSqlFilter::onSelectedFormShow);
    connect(this, &DialogSqlFilter::itemComboCurrentIndexChanged, this, &DialogSqlFilter::onItemComboCurrentIndexChanged);

    loadFilterItems();

    if(filterManager->loadCache() && !filterManager->nameOptions().isEmpty())
        setWindowTitle(QString("Настройка отборов (%1)").arg(filterManager->nameOptions()));


    connect(_filterManager, &FilterManager::resetFilter, this, &DialogSqlFilter::onResetFilter);

}

DialogSqlFilter::~DialogSqlFilter()
{
    delete ui;
}

void DialogSqlFilter::addFilter(const QString &filter_field, const QUuid& uuid)
{

    QTableWidget * table = ui->tableWidget;

    int rowCount = table->rowCount() + 1;
    int currentRow = rowCount - 1;

    table->setRowCount(rowCount);

    auto *pItemUuid = new QTableWidgetItem(uuid.toString());
    table->setItem(currentRow,4,pItemUuid);

    auto *pItemKey = new QTableWidgetItem(QString::number(currentRow));
    table->setItem(currentRow,5,pItemKey);

    auto *pWidget = new QWidget();
    auto *pCheckBox = new QCheckBox();
    auto *pLayout = new QHBoxLayout(pWidget);
    pLayout->addWidget(pCheckBox);
    pLayout->setAlignment(Qt::AlignCenter);
    pLayout->setContentsMargins(0,0,0,0);
    pWidget->setLayout(pLayout);
    table->setCellWidget(table->rowCount()-1,0,pWidget);
    pCheckBox->setCheckState(Qt::CheckState::Checked);
    pCheckBox->setObjectName("use" + QString::number(currentRow));

    auto itemField = new QTableWidgetItem(filter_field);
    itemField->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
    table->setItem(table->rowCount()-1, 1, itemField);

    /////////////
    auto list_model = new QStringListModel(szComparisonType);
    if(filter_field == "Дата"){
        list_model->removeRows(6, 2);
    }
    auto* combo = new QComboBox();
    combo->setModel(list_model);
    combo->setFrame(false);
    table->setCellWidget(table->rowCount()-1,2,combo);
    //////////////

    auto *pWidgetVal = new QWidget();
    if(filter_field == "Дата")
       pWidgetVal->setProperty("dataType", "date");
    else
       pWidgetVal->setProperty("dataType", "string");
    QStringList lst{};
    pWidgetVal->setProperty("list", lst);
    pWidgetVal->setProperty("isList", false);


    auto *pToolBtn = new QToolButton();
    auto *pLayoutVal = new QHBoxLayout(pWidgetVal);
    auto *pLineEdit = new QLineEdit();
    auto * pDateTime = new QDateTimeEdit(QDateTime::currentDateTime());

    pToolBtn->setAutoRaise(true);
    pToolBtn->setText("...");
    pToolBtn->setProperty("row", currentRow);
    pToolBtn->setObjectName("btn" + QString::number(currentRow));

    pDateTime->setObjectName("dateTime" + QString::number(currentRow));
    pDateTime->setFrame(false);
    pDateTime->setProperty("row", currentRow);
    pDateTime->setCalendarPopup(true);

    pLineEdit->setObjectName("lineEdit" + QString::number(currentRow));
    pLineEdit->setFrame(false);


    if(filter_field == "Дата"){
        pLineEdit->setVisible(false);
        pLineEdit->setEnabled(false);
        pToolBtn->setVisible(false);
        pWidgetVal->setProperty("code", pDateTime->dateTime());
    }
    else
        pDateTime->setVisible(false);

    pLayoutVal->addWidget(pDateTime);
    pLayoutVal->addWidget(pLineEdit);
    pLayoutVal->addWidget(pToolBtn);
    pLayoutVal->setContentsMargins(0,0,0,0);
    pWidgetVal->setLayout(pLayoutVal);

    table->setCellWidget(table->rowCount()-1,3,pWidgetVal);
    table->resizeColumnsToContents();

    connect(pToolBtn, SIGNAL(clicked()), this, SLOT(onToolButtonToggle()));
    connect(pDateTime, SIGNAL(dateTimeChanged(QDateTime)), this, SLOT(onDateTimeChanged(QDateTime)));
    connect(combo, QOverload<int>::of(&QComboBox::currentIndexChanged),
        [=](int index){
        emit itemComboCurrentIndexChanged(index);
    });
}

void DialogSqlFilter::updateManager()
{

    _filterManager->filterItems().clear();
   // qDebug() << _filterManager->uuid();

    int rows = ui->tableWidget->rowCount();

    for(auto i = 0; i < rows; i++){

        int keyString = ui->tableWidget->item(i, 5)->text().toInt();

        QWidget* pWidget = ui->tableWidget->cellWidget(i, 0);
        auto * checkBox = pWidget->findChild<QCheckBox*>("use" + QString::number(keyString));

        QWidget* pWidgetVal = ui->tableWidget->cellWidget(i, 3);
        QVariant mCode = pWidgetVal->property("listCode");
        QVariant vCode = pWidgetVal->property("code");

        QStringList lstCode{};
        if(mCode.isValid()){
            lstCode = mCode.toStringList();
        }


        bool isList = pWidgetVal->property("isList").toBool();

        bool isUse = false;
        if(checkBox)
            isUse = checkBox->checkState() == Qt::CheckState::Checked;

        QString filed = ui->tableWidget->item(i, 1)->text();
        if(filed.isEmpty()){
            qDebug() << "error field name in " << i << " row";
            continue;
        }

        auto itr = _aliasesField.find(filed);
        if(itr != _aliasesField.end())
            filed = itr.value();

        auto colIndex = (LogEventColumn)ColumnNames.indexOf(filed);
        auto *combo = dynamic_cast<QComboBox*>( ui->tableWidget->cellWidget(i, 2) );
        ComparisonType compareType = ComparisonType::equals;
        if(combo)
          compareType = _compareType[combo->currentText()];

        QVariant value;
        if(isList)
            value = lstCode;
        else{
            value = vCode;
        }

        auto *line = pWidgetVal->findChild<QLineEdit*>("lineEdit" + QString::number(keyString));

        if(!line){
            qDebug() << "error get item control row";
            continue;
        }

        QTableWidgetItem * item = ui->tableWidget->item(i, 4);

        _filterManager->setFilter(colIndex, compareType, value, isUse, line->text(), QUuid::fromString(item->text()));

    }

}

void DialogSqlFilter::loadFilterItems()
{
    ui->tableWidget->setRowCount(0);

    int row = 0;
    for (auto itr : _filterManager->filterItems()) {
        QString field;
        auto itrField = _aliasesField.find(itr->key());
        if(itrField != _aliasesField.end())
            field = itrField.value();

        addFilter(field, itr->uuid());

        QWidget* pWidget = ui->tableWidget->cellWidget(row, 0);
        auto * checkBox = pWidget->findChild<QCheckBox*>("use" + QString::number(row));
        if(checkBox){
            if(itr->use())
                checkBox->setCheckState(Qt::CheckState::Checked);
            else
                checkBox->setCheckState(Qt::CheckState::Unchecked);
        }

        auto *combo = dynamic_cast<QComboBox*>( ui->tableWidget->cellWidget(row, 2) );
        if(combo){
            for (auto compare = _compareType.begin(); compare != _compareType.end(); ++compare) {
                if(compare .value()== itr->compareType()){
                    int ind = (int)szComparisonType.indexOf(compare.key());
                    combo->setCurrentIndex(ind);
                    break;
                }
            }

        }

        QWidget* pWidgetVal = ui->tableWidget->cellWidget(row, 3);
        if(itr->value().isValid()){

            auto *line = pWidgetVal->findChild<QLineEdit*>("lineEdit" + QString::number(row));
            auto *dt = pWidgetVal->findChild<QDateTimeEdit*>("dateTime" + QString::number(row));

            if(itr->aliasesValue().isValid()){
                if(itr->aliasesValue().userType() == QMetaType::QString){
                    if(line)
                        line->setText(itr->aliasesValue().toString());
                    QStringList list = itr->aliasesValue().toString().split(",");
                    pWidgetVal->setProperty("list", list);
                }
            }

            if(itr->value().userType() == QMetaType::QString){
                pWidgetVal->setProperty("code", itr->value().toString());
                pWidgetVal->setProperty("isList", false);
            }else if(itr->value().userType() == QMetaType::QStringList){
                pWidgetVal->setProperty("listCode", itr->value().toStringList());
                pWidgetVal->setProperty("isList", true);
                if(line)
                    line->setEnabled(false);
            }else if(itr->value().userType() == QMetaType::QDateTime){
                pWidgetVal->setProperty("code", itr->value().toDateTime());
                pWidgetVal->setProperty("isList", false);
                if(dt)
                   dt->setDateTime(itr->value().toDateTime());
            }
        }

        row++;
    }
}


void DialogSqlFilter::onToolButtonToggle()
{
    auto *button = dynamic_cast<QToolButton*>( sender() );
    QVariant vRow = button->property("row");

    if(vRow.isValid()){

        int row = vRow.toInt();
        int curRow = ui->tableWidget->selectionModel()->currentIndex().row();
        if(row != curRow)
            ui->tableWidget->selectRow(row);

        QString field = ui->tableWidget->item(row, 1)->text();

        QWidget* pWidgetVal = ui->tableWidget->cellWidget(row, 3);

        auto *combo = dynamic_cast<QComboBox*>( ui->tableWidget->cellWidget(row, 2) );
        ComparisonType compareType = _compareType[combo->currentText()];

        emit selectedFormShow(compareType, pWidgetVal, field, row);


    }
}


void DialogSqlFilter::on_btnFilterItemDelete_clicked()
{
    if(ui->tableWidget->currentRow() >= 0){
        ui->tableWidget->removeRow(ui->tableWidget->currentRow());
        if(ui->tableWidget->rowCount() > 0){
            ui->tableWidget->setCurrentCell(0,0);
            ui->tableWidget->setCurrentItem(ui->tableWidget->item(0,0));
        }
    }
}


void DialogSqlFilter::on_tableWidget_doubleClicked(const QModelIndex &index)
{

}

void DialogSqlFilter::onSelectedFormShow(ComparisonType type, QWidget *pWidgetVal, const QString& field, int row)
{

    if(pWidgetVal){

        int keyString = ui->tableWidget->item(row, 5)->text().toInt();

        auto *line = pWidgetVal->findChild<QLineEdit*>("lineEdit" + QString::number(keyString));
        auto *dt = pWidgetVal->findChild<QDateTimeEdit*>("dateTime" + QString::number(keyString));

        QString tableName;

        auto itr = _aliases.find(field);
        if(itr != _aliases.end())
            tableName = itr.value();

        QStringList lst = pWidgetVal->property("list").toStringList();
        bool isList = pWidgetVal->property("isList").toBool();
        QString dataType = pWidgetVal->property("dataType").toString();

        auto dlg = new DialogSelectItemsFilter(this, tableName, field, lst, type);
        dlg->setModal(true);
        dlg->exec();
        if(dlg->result() == QDialog::Accepted){
            if(!isList){
                if(dlg->selectedValue.userType() == QMetaType::QString){
                    QString val = dlg->selectedValue.toString();

                    if(dataType == "string")
                    {
                        pWidgetVal->setProperty("code", dlg->selectedCode.toString());
                        if(line)
                            line->setText(val);
                    }else{
                        if(dt){
                            dt->setDateTime(QDateTime::fromString(val));
                        }
                    }
                }

            }else{
                if(dlg->selectedValue.userType() == QMetaType::QStringList){
                    lst = dlg->selectedValue.toStringList();
                    QStringList LstCode = dlg->selectedCode.toStringList();
                    pWidgetVal->setProperty("list", lst);
                    pWidgetVal->setProperty("listCode", LstCode);
                    if(line)
                        line->setText(lst.join(","));
                }
            }

        }

    }

    qDebug() << field;
}

void DialogSqlFilter::onItemComboCurrentIndexChanged(int index)
{
    int row = ui->tableWidget->currentRow();

    bool isList = false;
    if(index > 5 )
        isList = true;
    QWidget* pWidgetVal = ui->tableWidget->cellWidget(row, 3);

    if(pWidgetVal  && (isList != pWidgetVal->property("isList").toBool())){

        int keyString = ui->tableWidget->item(row, 5)->text().toInt();

        auto *dt = pWidgetVal->findChild<QDateTimeEdit*>("dateTime" + QString::number(keyString));
        auto *btn = pWidgetVal->findChild<QToolButton*>("btn" + QString::number(keyString));
        auto *line = pWidgetVal->findChild<QLineEdit*>("lineEdit" + QString::number(keyString));

        QStringList lst = pWidgetVal->property("list").toStringList();
        QStringList lstCode = pWidgetVal->property("listCode").toStringList();
        QVariant code = pWidgetVal->property("code");

        if(pWidgetVal->property("dataType") == "date"){

            if(dt)
            {
                dt->setVisible(!isList);
                line->setVisible(isList);
                btn->setVisible(isList);

                if(isList){
                    line->setText(dt->text());
                    lst.clear();
                    lst.append(dt->text());
                }else{
                    if(lst.size() > 0){
                        QDateTime date = QDateTime::fromString(lst[0]);
                        dt->setDateTime(date);
                    }

                }
            }

        }else{
            if(line){
                line->setEnabled(!isList);
                if(isList){
                    lst.clear();
                    if(!line->text().isEmpty()){
                        lst.append(line->text());
                        pWidgetVal->setProperty("list", lst);
                        if(code.isValid()){
                            lstCode.clear();
                            lstCode.append(code.toString());
                            pWidgetVal->setProperty("listCode", lstCode);
                        }
                    }
                }else{
                    if(!lst.empty()){
                        line->setText(lst[0]);
                    }
                    if(!lstCode.empty()){
                        pWidgetVal->setProperty("code", lstCode[0]);
                        pWidgetVal->setProperty("listCode", lstCode);
                    }
                }
            }
        }

        pWidgetVal->setProperty("isList", isList);
    }

}


void DialogSqlFilter::on_buttonBox_accepted()
{
    updateManager();
}

void DialogSqlFilter::on_btnFilterItemAdd_clicked()
{

    auto dlg = new SelectionDialog(this, ColumnNamesAliases, "Поля отбора");
    dlg->setModal(true);
    dlg->exec();
    if(dlg->result() == QDialog::Accepted){
        QVariant res = dlg->getResult();
        if(res.userType() == QMetaType::QString){
            addFilter(res.toString());

        }
    }
}


void DialogSqlFilter::on_btnSaveFilter_clicked()
{
    updateManager();

    auto dlg = new DialogSaveFilter(_filterManager, false, this);
    dlg->setModal(true);
    dlg->exec();
    if(dlg->result() == QDialog::Accepted){
        if(_filterManager->loadCache() && !_filterManager->nameOptions().isEmpty())
            setWindowTitle(QString("Настройка отборов (%1)").arg(_filterManager->nameOptions()));
    }
}


void DialogSqlFilter::on_btnLoadFilter_clicked()
{
    auto dlg = new DialogSaveFilter(_filterManager, true, this);
    dlg->setModal(true);
    dlg->exec();
    if(dlg->result() == QDialog::Accepted){
        QUuid source = dlg->getResult();
        _filterManager->load(source);
    }
}

void DialogSqlFilter::onResetFilter()
{
    loadFilterItems();
    setWindowTitle(QString("Настройка отборов (%1)").arg(_filterManager->nameOptions()));
}

void DialogSqlFilter::onDateTimeChanged(const QDateTime &dateTime)
{
    auto *dt = dynamic_cast<QDateTimeEdit*>( sender() );
    QVariant vRow = dt->property("row");
    if(vRow.isValid()){
        int row = vRow.toInt();
        QWidget* pWidgetVal = ui->tableWidget->cellWidget(row, 3);
        if(pWidgetVal){
            pWidgetVal->setProperty("code", dateTime);
            auto *line = pWidgetVal->findChild<QLineEdit*>("lineEdit" + QString::number(row));
            if(line)
                line->setText(dateTime.toString());
        }
    }
}

