#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QMap>
#include "infobases.h"

const QStringList ColumnNames ={"rowID",
                                "severity",
                                "date",
                                "connectID",
                                "session",
                                "transactionStatus",
                                "transactionDate",
                                "transactionID",
                                "userCode",
                                "computerCode",
                                "appCode",
                                "eventCode",
                                "comment",
                                "metadataCodes",
                                "sessionDataSplitCode",
                                "dataType",
                                "data",
                                "dataPresentation",
                                "workServerCode",
                                "primaryPortCode",
                                "secondaryPortCode"};

class Settings // : public QObject
{
    //Q_OBJECT

public:


    explicit Settings();//QObject *parent = nullptr);

#ifdef _WINDOWS
    void get_server_info(const QString& dirsrvinfo = "");
#else
    void get_server_info(const QString& dirsrvinfo);
#endif
    static QStringList ParseEventLogString(const QString& text);
    static double CountSubstringInString(const QString& str, const QString& substr);

    //QString v8srvinfo;

#ifdef _WINDOWS
    static QString get_service_command_line();
    void parse_command_line(const QString& cmd, QString& result);
#endif

    void getSettings();
    QString v8srvinfo_catalog();

    void setV8Root(const QString& val){
       srv_v8info = val.toStdString();
    }

private:
    std::string srv_v8info;
    QMap<QString, Infobases*> info_bases;
    QMap<QString,bool> selectedCols;

//signals:

};

#endif // SETTINGS_H
