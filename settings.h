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

const QStringList ColumnNamesAliases ={ "Дата",
                                        "Приложение",
                                        "Событие",
                                        "Компьютер",
                                        "Комментарий",
                                        "Метаданные",
                                        "Пользователь",
                                        "dataType",
                                        "data",
                                        "dataPresentation",
                                        "rowID"};

enum LogEventColumn{
        rowID = 0,
        severity,
        date,
        connectID,
        session,
        transactionStatus,
        transactionDate,
        transactionID,
        userCode,
        computerCode,
        appCode,
        eventCode,
        comment,
        metadataCodes,
        sessionDataSplitCode,
        dataType,
        data,
        dataPresentation,
        workServerCode,
        primaryPortCode,
        secondaryPortCode
    };


class Settings : public QObject
{
    Q_OBJECT

public:

    explicit Settings(QObject *parent = nullptr);

    void getSettings();
    void saveSettings();

    QString v8srvinfo_catalog() const;
    void set_v8srvinfo_catalog(const QString& val);

    QString v8srvinfo_logevent_catalog();

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
    void parse_command_line(const QString& cmd, QString& result, QString& root_result);
#endif

    QMap<QString, Infobases*> get_infobases();
    QMap<QString,bool>& get_selected_cols();

    QString get_code_table_from_alias_index(int index);
    QString get_field_name_from_alias_index(int index);

private:
    QString _root_path;
    QString _v8srvinfo;
    QMap<QString, Infobases*> info_bases;
    QMap<QString,bool> selectedCols;
    QMap<int, QString> _code_tables_from_alias_index;
    QMap<int, QString> _fields_from_alias_index;

    void set_v8srvinfo_logevent_catalog(const QString& val);
//signals:

};

#endif // SETTINGS_H
