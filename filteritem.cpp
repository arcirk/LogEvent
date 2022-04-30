#include "filteritem.h"

FilterItem::FilterItem()
{
    _use = false;
}

void FilterItem::setKey(LogEventColumn index)
{
    _keyIndex = index;
    _key = ColumnNames[index];
}

void FilterItem::setValue(const QVariant &val)
{
    _value = val;
}

void FilterItem::setAliasesValue(const QVariant &val)
{
    _valueAliases = val;
}

void FilterItem::setCompareType(ComparisonType type)
{
    _compareType = type;
}

void FilterItem::setUse(bool val)
{
    _use = val;
}

LogEventColumn FilterItem::keyIndex()
{
    return _keyIndex;
}

QVariant FilterItem::value()
{
    return _value;
}

QVariant FilterItem::aliasesValue()
{
    return _valueAliases;
}

QString FilterItem::key()
{
    return _key;
}

ComparisonType FilterItem::compareType()
{
    return _compareType;
}

bool FilterItem::use()
{
    return _use;
}
