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
    logDbPath = "";
    loadAppSettings();
    ui->linePath->setText(logDbPath);
}

DialogOptions::~DialogOptions()
{
    delete ui;
}

void DialogOptions::on_btnSelectFile_clicked()
{
    QString file = QFileDialog::getOpenFileName(this);
    if(!file.isEmpty()){
        logDbPath = file;
       ui->linePath->setText(logDbPath);
    }
}


void DialogOptions::on_buttonBox_accepted()
{
    logDbPath = ui->linePath->text();
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
        auto _logDbPateh = obj.find("logDbPath");
        if(_logDbPateh->isString()){
            logDbPath = _logDbPateh->toString();
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
    obj.insert("logDbPath", logDbPath);

    QJsonDocument _doc(obj);
    jsonFile.write(_doc.toJson(QJsonDocument::Indented));
    jsonFile.close();
}

