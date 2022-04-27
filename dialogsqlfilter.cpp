#include "dialogsqlfilter.h"
#include "ui_dialogsqlfilter.h"

#include "dialogselectitemsfilter.h"

DialogSqlFilter::DialogSqlFilter(QWidget *parent, QSqlDatabase& db, const QString& saveOptions) :
    QDialog(parent),
    ui(new Ui::DialogSqlFilter),
    m_db(db)
{
    ui->setupUi(this);

    _save_options = saveOptions;
}

DialogSqlFilter::~DialogSqlFilter()
{
    delete ui;
}

void DialogSqlFilter::on_chSelApp_toggled(bool checked)
{
    ui->lineSelApp->setEnabled(checked);
    ui->btnSelApp->setEnabled(checked);
    ui->btnSelAppClear->setEnabled(checked);
}


void DialogSqlFilter::on_btnSelApp_clicked()
{
    auto dlg = new DialogSelectItemsFilter(this, "AppCodes", m_db, _save_options);
    dlg->setModal(true);
    dlg->exec();
    if(dlg->result() == QDialog::Accepted){

    }
}

