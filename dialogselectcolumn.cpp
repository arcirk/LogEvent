#include "dialogselectcolumn.h"
#include "ui_dialogselectcolumn.h"

DialogSelectColumn::DialogSelectColumn(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSelectColumn)
{
    ui->setupUi(this);
}

DialogSelectColumn::DialogSelectColumn(QWidget *parent, Settings * option):
    QDialog(parent),
    ui(new Ui::DialogSelectColumn)
{
    ui->setupUi(this);

    model = new QStandardItemModel(this);
    model->setColumnCount(2);
    auto * itemName = new QStandardItem("Имя");
    model->setHorizontalHeaderItem(0, itemName);
    auto * itemVal = new QStandardItem("Видимость");
    model->setHorizontalHeaderItem(1, itemVal);

    model->setRowCount(option->get_selected_cols().size());
    int i = 0;

    for (auto itr = option->get_selected_cols().begin(); itr != option->get_selected_cols().end() ; ++itr) {
        auto * item = new QStandardItem(itr.key());
        model->setItem(i, 0, item);
        item->setEditable(false);
        auto * itemV = new QStandardItem(itr.value());
        itemV->setCheckable(true);
       if(itr.value()){
            itemV->setCheckState(Qt::CheckState::Checked);
       }
        model->setItem(i, 1, itemV);
        i++;
    }

    ui->tableView->setModel(model);
    ui->tableView->setColumnWidth(0, this->width()/2);

    _option = option;
}

DialogSelectColumn::~DialogSelectColumn()
{
    delete ui;
}


void DialogSelectColumn::on_buttonBox_accepted()
{

    for (auto i = 0; i < model->rowCount(); i++) {
        QStandardItem * item = model->item(i, 1);
        QStandardItem * itemName = model->item(i, 0);
        auto itr = _option->get_selected_cols().find(itemName->text());
        if(itr != _option->get_selected_cols().end()){
            bool val = item->checkState() == Qt::CheckState::Checked;
            _option->get_selected_cols()[itemName->text()] = val;
        }


    }
}

