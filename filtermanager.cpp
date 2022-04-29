#include "filtermanager.h"

FilterManager::FilterManager()
{

}

void FilterManager::setFilter(LogEventColumn colIndex, ComparisonType compareType, const QVariant &vals)
{
    auto filter = new FilterItem();
    filter->setKey(colIndex);
    filter->setCompareType(compareType);
    filter->setValue(vals);

    _filterItems.insert(colIndex, filter);

}

QMap<LogEventColumn, FilterItem *>& FilterManager::filterItems()
{
    return _filterItems;
}
