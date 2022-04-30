#include "selectiondialog.h"
#include "ui_selectiondialog.h"
#include <QStringListModel>
#include <QAbstractItemView>

SelectionDialog::SelectionDialog(QWidget *parent, const QStringList& list, const QString& title) :
    QDialog(parent),
    ui(new Ui::SelectionDialog)
{
    ui->setupUi(this);

    setWindowTitle(title);

    auto  m_list = new QStringListModel(list);
    ui->listView->setModel(m_list);
    ui->listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

SelectionDialog::~SelectionDialog()
{
    delete ui;
}

QVariant SelectionDialog::getResult() const
{
    return selection_result;
}


void SelectionDialog::on_buttonBox_accepted()
{
    selection_result = ui->listView->selectionModel()->currentIndex().data(Qt::DisplayRole).toString();
}


void SelectionDialog::on_listView_doubleClicked(const QModelIndex &index)
{
    selection_result = index.model()->data(index, Qt::DisplayRole).toString();
    accept();
}

