#include "dialogselectedrow.h"
#include "ui_dialogselectedrow.h"

DialogSelectedRow::DialogSelectedRow(QMap<QString, QString> row, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSelectedRow)
{
    ui->setupUi(this);

    auto itr = row.find("date");
    if(itr != row.end()){
        ui->lblDate->setText(itr.value());
        ui->lblDate->setReadOnly(true);
    }
    itr = row.find("app");
    if(itr != row.end()){
        ui->lblApp->setText(itr.value());
        ui->lblApp->setReadOnly(true);
    }
    itr = row.find("event");
    if(itr != row.end()){
        ui->lblEvent->setText(itr.value());
        ui->lblEvent->setReadOnly(true);
    }
    itr = row.find("comp");
    if(itr != row.end()){
        ui->lblComp->setText(itr.value());
        ui->lblComp->setReadOnly(true);
    }
    itr = row.find("comment");
    if(itr != row.end()){
        ui->txtComment->setText(itr.value());
        ui->txtComment->setReadOnly(true);
    }
    itr = row.find("metadata");
    if(itr != row.end()){
        ui->txtMetadata->setText(itr.value());
        ui->txtMetadata->setReadOnly(true);
    }
    itr = row.find("user");
    if(itr != row.end()){
        ui->lblUser->setText(itr.value());
        ui->lblUser->setReadOnly(true);
    }

}

DialogSelectedRow::~DialogSelectedRow()
{
    delete ui;
}
