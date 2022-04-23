#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QMap>
#include "infobases.h"

#ifdef WINDOWS
#include <QAxObject>
#endif

class Settings : public QObject
{
    Q_OBJECT
public:
    explicit Settings(QObject *parent = nullptr);

    QString databasePath;
    QMap<int, bool> columnVisible;

    void setServerLogFolder(const QString& path);

    void get_server_info();

    static QStringList ParseEventLogString(const QString& text);
    static double CountSubstringInString(const QString& str, const QString& substr);


private:
    QString serverLogFolder;
    QMap<QString, Infobases*> info_bases;

signals:

};

#endif // SETTINGS_H
