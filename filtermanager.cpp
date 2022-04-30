#include "filtermanager.h"

FilterManager::FilterManager()
{

}

void FilterManager::setFilter(LogEventColumn colIndex, ComparisonType compareType, const QVariant &vals, bool use, const QVariant& alias_vals)
{
    auto filter = new FilterItem();
    filter->setKey(colIndex);
    filter->setCompareType(compareType);
    filter->setValue(vals);
    filter->setUse(use);
    filter->setAliasesValue(alias_vals);
    _filterItems.insert(colIndex, filter);

}

QMap<LogEventColumn, FilterItem *>& FilterManager::filterItems()
{
    return _filterItems;
}
