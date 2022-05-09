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
    bool def;

    explicit filter_options(){};
    ~filter_options(){};

};

class FilterManager : public QObject
{
    Q_OBJECT

public:
    explicit FilterManager(QObject *parent = nullptr);
    ~FilterManager() override{};

    void setFilter(LogEventColumn  colIndex,
                   ComparisonType compareType,
                   const QVariant& vals, bool use,
                   const QVariant& alias_vals,
                   const QUuid& id = QUuid::createUuid());

    QMap<QUuid, FilterItem*>& filterItems();

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



    void setSaveCache(bool val);
    bool saveCache();
    void setloadCache(bool val);
    bool loadCache();

    void reset();
    void load(const QUuid& uuid);

    QList<FilterItem*> findItemByName(const QString& name);

private:
    QMap<QUuid, FilterItem*> _filterItems;
    QString _databaseName;
    QString _nameOptions;
    QUuid _uuid;
    bool _saveCache;
    bool _loadCache;

signals:

    void updateFilterOptions(const QString& uuid, const QString& name);
    void updateAllFilterOptions(QList<filter_options*>& values);
    void removeItemOptions(const QUuid& id);
    void copyItemOptions(const QUuid& source, const QUuid& result, const QString& name);
    void loadItemOptions(const QUuid& id);
    void resetFilter();
};

#endif // FILTERMANAGER_H
