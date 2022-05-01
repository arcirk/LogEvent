#ifndef FILTERMANAGER_H
#define FILTERMANAGER_H

#include <QJsonDocument>
#include <QObject>
#include "settings.h"
#include "querybuilder.h"
#include "filteritem.h"
#include <QMap>
#include <QUuid>

struct filter_options{
public:
    QString uuid;
    QString name;
    bool save;
    bool load;

    explicit filter_options(){};
    ~filter_options(){};

};

class FilterManager : public QObject
{
    Q_OBJECT

public:
    explicit FilterManager(QObject *parent = nullptr);
    ~FilterManager(){};

    void setFilter(LogEventColumn  colIndex, ComparisonType compareType, const QVariant& vals, bool use, const QVariant& alias_vals);
    QMap<LogEventColumn, FilterItem*>& filterItems();

    void setRandomUuid();
    void setUuid(const QString& uuid);
    QUuid uuid();

    QString nameOptions();
    void setNameOptions(const QString& name);

    QString databaseName();
    void setDatabaseName(const QString& name);

    QString toString();
    QJsonObject toJsonObject();
    void setFiltersCache(QJsonObject cache);

    void init(const QString& uuid);

    void setSaveCache(bool val);
    bool saveCache();
    void setloadCache(bool val);
    bool loadCache();

private:
    QMap<LogEventColumn, FilterItem*> _filterItems;
    QString _databaseName;
    QString _nameOptions;
    QUuid _uuid;
    bool _saveCache;
    bool _loadCache;

signals:

    void updateFilterOptions(const QString& uuid, const QString& name);
    void updateAllFilterOptions(QList<filter_options*>& values);
};

#endif // FILTERMANAGER_H
