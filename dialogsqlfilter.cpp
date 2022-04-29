#include "dialogsqlfilter.h"
#include "ui_dialogsqlfilter.h"
#include <QTableWidgetItem>
#include <QTableWidget>
#include "dialogselectitemsfilter.h"
#include "settings.h"
#include <QStandardItem>

DialogSqlFilter::DialogSqlFilter(QWidget *parent, FilterManager * filterManager) :
    QDialog(parent),
    ui(new Ui::DialogSqlFilter)
{
    ui->setupUi(this);
//    setControls();
//    _filterManager = filterManager;
//    updateData();

    QTableWidget* table = ui->tableWidget;
    table->setColumnCount(4);
    table->setHorizontalHeaderItem(0, new QTableWidgetItem(""));
    table->setHorizontalHeaderItem(1, new QTableWidgetItem("Поле"));
    table->setHorizontalHeaderItem(2, new QTableWidgetItem("Вид сравнения"));
    table->setHorizontalHeaderItem(3, new QTableWidgetItem("Значение"));


    ui->toolBarFilter->addStretch();

    m_model = new QStandardItemModel();
    m_model->setColumnCount(2);
    auto itemSelect = new QStandardItem("");
    m_model->setHorizontalHeaderItem(0, itemSelect);
    auto itemField = new QStandardItem("Поле");
    m_model->setHorizontalHeaderItem(1, itemField);
    ui->tableView->setModel(m_model);
    ui->tableView->setColumnWidth(0, 24);
    ui->tableView->setColumnWidth(1, 80);

}

DialogSqlFilter::~DialogSqlFilter()
{
    delete ui;
}


//void DialogSqlFilter::on_btnSelApp_clicked()
//{
//    auto dlg = new DialogSelectItemsFilter(this, "AppCodes");
//    dlg->setModal(true);
//    dlg->exec();
//    if(dlg->result() == QDialog::Accepted){

//    }
//}

void DialogSqlFilter::setControls()
{
//    for (int i = 0; i < ColumnNames.size(); i++){
//        switch ((LogEventColumn)i) {
//        case LogEventColumn::appCode:{
//            auto itemUi = new filterItemUi();
//            itemUi->checkBox = ui->chSelApp;
//            itemUi->lineEdit = ui->lineSelApp;
//            m_controls.insert((LogEventColumn)i, itemUi);
//            break;
//            }

//        }
//    }
}

void DialogSqlFilter::updateData()
{

//    for (auto itr : _filterManager->filterItems()) {
//        Qt::CheckState state = Qt::CheckState::Unchecked;
//        if(itr->use())
//            state = Qt::CheckState::Checked;

//        m_controls[itr->keyIndex()]->checkBox->setCheckState(state);

//        QVariant value = itr->value();

//        if(value.userType() == QMetaType::QString)
//            m_controls[itr->keyIndex()]->lineEdit->setText(value.toString());
//        else if (value.userType() == QMetaType::QStringList){
//            QStringList val = value.toStringList();
//            m_controls[itr->keyIndex()]->lineEdit->setText(val.join(","));
//        }
//    }

}

