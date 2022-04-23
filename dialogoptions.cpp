#include "dialogoptions.h"
#include "ui_dialogoptions.h"
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFile>
#include <QFileDialog>

DialogOptions::DialogOptions(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogOptions)
{    
    ui->setupUi(this);
    logDbPateh = "";
    loadAppSettings();
    ui->linePath->setText(logDbPateh);
}

DialogOptions::~DialogOptions()
{
    delete ui;
}

void DialogOptions::on_btnSelectFile_clicked()
{
    QString file = QFileDialog::getOpenFileName(this);
    if(!file.isEmpty()){
       logDbPateh = file;
       ui->linePath->setText(logDbPateh);
    }
}


void DialogOptions::on_buttonBox_accepted()
{
    logDbPateh = ui->linePath->text();
    saveAppSettings();
}

void DialogOptions::loadAppSettings()
{

    QFile file("settings.json");
    if (!file.open(QIODevice::ReadOnly))
        return;

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    if(!doc.isNull()){
        QJsonObject obj = doc.object();
        auto _logDbPateh = obj.find("logDbPateh");
        if(_logDbPateh->isString()){
            logDbPateh = _logDbPateh->toString();
        }
    }
    file.close();
}

void DialogOptions::saveAppSettings()
{
    QFile jsonFile("settings.json");
    if (!jsonFile.open(QIODevice::WriteOnly))
    {
        return;
    }

    QJsonObject obj = QJsonObject();
    obj.insert("logDbPateh", logDbPateh);

    QJsonDocument _doc(obj);
    jsonFile.write(_doc.toJson(QJsonDocument::Indented));
    jsonFile.close();
}

