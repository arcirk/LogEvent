#include "dialogselectcolumn.h"
#include "ui_dialogselectcolumn.h"
#include <QStandardItemModel>

DialogSelectColumn::DialogSelectColumn(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSelectColumn)
{
    ui->setupUi(this);
}

DialogSelectColumn::DialogSelectColumn(QWidget *parent, QMap<QString, bool> &option):
    QDialog(parent),
    ui(new Ui::DialogSelectColumn)
{
    ui->setupUi(this);

    auto model = new QStandardItemModel(this);
    model->setColumnCount(2);
    QStandardItem * itemName = new QStandardItem("Имя");
    model->setHorizontalHeaderItem(0, itemName);
    QStandardItem * itemVal = new QStandardItem("Видимость");
    model->setHorizontalHeaderItem(1, itemVal);

    model->setRowCount(option.size());
    int i = 0;

    for (auto itr = option.begin(); itr != option.end() ; ++itr) {
        QStandardItem * item = new QStandardItem(itr.key());
        model->setItem(i, 0, item);
        QStandardItem * itemV = new QStandardItem(itr.value());
        model->setItem(i, 1, itemV);
        i++;
    }

    ui->tableView->setModel(model);
}

DialogSelectColumn::~DialogSelectColumn()
{
    delete ui;
}
