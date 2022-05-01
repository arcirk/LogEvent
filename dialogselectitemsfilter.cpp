#include "dialogselectitemsfilter.h"
#include "ui_dialogselectitemsfilter.h"
#include <QSqlQueryModel>
#include "electionlistsmodel.h"
#include <QSqlQuery>
#include <QStandardItemModel>
#include <QMessageBox>

DialogSelectItemsFilter::DialogSelectItemsFilter(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSelectItemsFilter)
{
    ui->setupUi(this);
}

DialogSelectItemsFilter::DialogSelectItemsFilter(QWidget *parent, const QString &table, const QStringList& lst, ComparisonType compareType) :
    QDialog(parent),
    ui(new Ui::DialogSelectItemsFilter)
{
    ui->setupUi(this);

    model = new QStandardItemModel(this);
    model->setColumnCount(2);

    QStandardItem * itemVal = new QStandardItem("");
    model->setHorizontalHeaderItem(0, itemVal);
    QStandardItem * itemName = new QStandardItem("Значение");
    model->setHorizontalHeaderItem(1, itemName);
    QStandardItem * itemCode = new QStandardItem("Код");
    model->setHorizontalHeaderItem(2, itemCode);



    if(!table.isEmpty())
        init_sql_table(table, lst);
    else
        init_table(lst);

    ui->tableView->setModel(model);
    //ui->tableView->setColumnHidden(2, true);
    ui->tableView->resizeColumnsToContents();

    isList = false;

    if(compareType != ComparisonType::not_in_list && compareType != ComparisonType::on_list){
        ui->tableView->setColumnHidden(0, true);
        ui->btnCheckedAll->setVisible(false);
        ui->btnUncheckedAll->setVisible(false);
    }else
        isList = true;

    ui->bToolBar->addStretch();

    setWindowTitle(table);
}

DialogSelectItemsFilter::~DialogSelectItemsFilter()
{
    delete ui;
}

void DialogSelectItemsFilter::on_btnUncheckedAll_clicked()
{
    checked_unchecked(Qt::CheckState::Unchecked);
}


void DialogSelectItemsFilter::on_btnCheckedAll_clicked()
{
    checked_unchecked(Qt::CheckState::Checked);
}

void DialogSelectItemsFilter::checked_unchecked(Qt::CheckState value)
{
    for (auto i = 0; i < model->rowCount(); i++) {
        QStandardItem * item = model->item(i, 0);
        item->setCheckState(value);


    }
}

void DialogSelectItemsFilter::init_sql_table(const QString& table, const QStringList& lst)
{
    QString query = QString("select false as sel, name, code from %1;").arg(table);

    QSqlQuery _query;
    _query.exec(query);

//    _query.last();
//    int row_count = _query.at() + 1;
//    _query.first();
//    model->setRowCount(row_count);

    int i = 0;
    while (_query.next()) {
        model->setRowCount(i+1);
        QStandardItem * sel = new QStandardItem();
        model->setItem(i, 0, sel);
        sel->setCheckable(true);

        QString name = _query.value(1).toString();
        QStandardItem * itemName = new QStandardItem(name);
        auto itr = lst.indexOf(name);
        if(itr >= 0)
            sel->setCheckState(Qt::CheckState::Checked);

        model->setItem(i, 1, itemName);

        QStandardItem * itemCode = new QStandardItem(QString::number(_query.value(2).toInt()));
        model->setItem(i, 2, itemCode);
        i++;
    }

}

void DialogSelectItemsFilter::init_table(const QStringList &lst)
{

}


void DialogSelectItemsFilter::on_tableView_doubleClicked(const QModelIndex &index)
{
    QString value = ui->tableView->model()->data(index, Qt::DisplayRole).toString();

    if(isList){
        auto * item = model->item(index.row(), 0);
        if(item->checkState() == Qt::CheckState::Checked)
           item->setCheckState(Qt::CheckState::Checked);
        else
           item->setCheckState(Qt::CheckState::Unchecked);

    }else{
        selectedValue = value;
        selectedCode = model->item(index.row(), 2)->text();
        accept();
    }

}


void DialogSelectItemsFilter::on_buttonBox_accepted()
{
    if(!isList){
        int row = ui->tableView->selectionModel()->currentIndex().row();
        if(row < 0){
            QMessageBox::critical(this, "Ошибка", "Не выбран элемент!");
            return;
        }
        selectedValue = model->item(row, 1)->text();
        selectedCode = model->item(row, 2)->text();
    }else{
        QStringList lst{};
        QStringList lstCode{};
        for (auto i = 0; i < model->rowCount(); i++) {

            QStandardItem * item = model->item(i, 0);
            if(item->checkState() == Qt::CheckState::Checked){
                lst.append(model->item(i, 1)->text());
                lstCode.append(model->item(i, 2)->text());
            }
            selectedValue = lst;
            selectedCode = lstCode;
        }
    }
}

