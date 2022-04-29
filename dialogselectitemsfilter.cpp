#include "dialogselectitemsfilter.h"
#include "ui_dialogselectitemsfilter.h"
#include <QSqlQueryModel>
#include "electionlistsmodel.h"
#include <QSqlQuery>
#include <QStandardItemModel>

DialogSelectItemsFilter::DialogSelectItemsFilter(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSelectItemsFilter)
{
    ui->setupUi(this);
}

DialogSelectItemsFilter::DialogSelectItemsFilter(QWidget *parent, const QString &table, QSqlDatabase& db, const QString& saveOptions) :
    QDialog(parent),
    ui(new Ui::DialogSelectItemsFilter)
{
    ui->setupUi(this);

    QString query = QString("select false as sel, name from %1;").arg(table);

//    auto model = new ElectionListsModel();
//    model->setQuery(query);

//    ui->tableView->setModel(model);

    QSqlQuery _query;
    _query.exec(query);

    model = new QStandardItemModel(this);
    model->setColumnCount(2);

    QStandardItem * itemVal = new QStandardItem("");
    model->setHorizontalHeaderItem(0, itemVal);
    QStandardItem * itemName = new QStandardItem("Имя");
    model->setHorizontalHeaderItem(1, itemName);
    QStandardItem * itemCode = new QStandardItem("Код");
    model->setHorizontalHeaderItem(2, itemCode);

    _query.last();
    int row_count = _query.at();
    _query.first();
    model->setRowCount(row_count);

    int i = 0;
    while (_query.next()) {
        QStandardItem * sel = new QStandardItem();
        model->setItem(i, 0, sel);
        sel->setCheckable(true);

        QStandardItem * itemName = new QStandardItem(_query.value(1).toString());
        model->setItem(i, 1, itemName);
        QStandardItem * itemCode = new QStandardItem(_query.value(0).toInt());
        model->setItem(i, 2, itemCode);
        i++;
    }

    ui->tableView->setModel(model);
    ui->tableView->setColumnHidden(2, true);
    ui->tableView->resizeColumnsToContents();
    ui->hlayout->addStretch();

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

