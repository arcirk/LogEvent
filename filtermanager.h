#ifndef FILTERMANAGER_H
#define FILTERMANAGER_H

#include <QJsonDocument>
#include <QObject>
#include "settings.h"
#include "querybuilder.h"
#include "filteritem.h"
#include <QMap>



class FilterManager
{
public:
    explicit FilterManager();

    void setFilter(LogEventColumn  colIndex, ComparisonType compareType, const QVariant& vals, bool use, const QVariant& alias_vals);
    QMap<LogEventColumn, FilterItem*>& filterItems();


private:
    QMap<LogEventColumn, FilterItem*> _filterItems;
};

#endif // FILTERMANAGER_H
