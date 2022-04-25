#include "dialogselectinfobase.h"
#include "ui_dialogselectinfobase.h"

DialogSelectInfobase::DialogSelectInfobase(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSelectInfobase)
{
    ui->setupUi(this);
}

DialogSelectInfobase::DialogSelectInfobase(QWidget *parent, QMap<QString, Infobases *> ib) :
    QDialog(parent),
    ui(new Ui::DialogSelectInfobase)
{

    ui->setupUi(this);



    QTableWidget * table = ui->tableInfoBase;
    table->setRowCount(0);

    table->setRowCount((int)ib.size());

    int index = 0;
    for (auto item : ib) {

        auto itemKey = new QTableWidgetItem(item->Name());
        itemKey->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        table->setItem(index, 0, itemKey);

        QTableWidgetItem *itemVal = new QTableWidgetItem(QString::number(item->SizeEventLog(), 'f', 2) + " mb");
        itemVal->setTextAlignment(Qt::AlignRight);
        itemVal->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        table->setItem(index, 1, itemVal);

        index++;
    }

//    table->horizontalHeader()->setResizeContentsPrecision(QHeaderView::Fixed);
//    for (int i = 0; i < table->columnCount() ; ++i) {
//        table->resizeColumnToContents(i);
//    }
}

DialogSelectInfobase::~DialogSelectInfobase()
{
    delete ui;
}

void DialogSelectInfobase::on_tableInfoBase_cellDoubleClicked(int row, int column)
{
      //qDebug() << "on_tableInfoBase_itemDoubleClicked";
      QTableWidgetItem *item = ui->tableInfoBase->item(row, 0);
      selectedItem = item->text();
      this->accept();
}


void DialogSelectInfobase::on_buttonBox_accepted()
{
    QTableWidgetItem *item = ui->tableInfoBase->item(ui->tableInfoBase->currentRow(), 0);
    selectedItem = item->text();
}

