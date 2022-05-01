#include "filtermanager.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

FilterManager::FilterManager(QObject *parent) :
    QObject(parent)
{
    setRandomUuid();
    _nameOptions = "unknown";
    _saveCache = false;
    _loadCache = false;
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

void FilterManager::setRandomUuid()
{
    _uuid = QUuid::createUuid();
}

void FilterManager::setUuid(const QString &uuid)
{
    _uuid = QUuid::fromString(uuid);
}

QUuid FilterManager::uuid()
{
    return _uuid;
}

QString FilterManager::nameOptions()
{
    return _nameOptions;
}

void FilterManager::setNameOptions(const QString &name)
{
    _nameOptions = name;
}

QString FilterManager::databaseName()
{
    return _databaseName;
}

void FilterManager::setDatabaseName(const QString &name)
{
    _databaseName = name;
}

QString FilterManager::toString()
{
    auto doc = QJsonDocument();
    auto obj = toJsonObject();
    doc.setObject(obj);
    return doc.toJson();
}

QJsonObject FilterManager::toJsonObject()
{
    auto obj = QJsonObject();

    obj.insert("uuid", _uuid.toString());
    obj.insert("nameOptions", _nameOptions);
    obj.insert("databaseName", _databaseName);
    obj.insert("saveCache", _saveCache);
    obj.insert("loadCache", _loadCache);

    auto arr = QJsonArray();
    for (auto itr : _filterItems) {
        auto item = QJsonObject();
        item.insert("key", itr->key());
        item.insert("keyIndex", itr->keyIndex());
        item.insert("compareType", itr->compareType());
        item.insert("use", itr->use());
        if(itr->value().userType() == QMetaType::QStringList){
            QStringList val = itr->value().toStringList();
            auto arrVal = QJsonArray();
            for (auto v : val) {
                arrVal.append(v);
            }
            item.insert("value", arrVal);
        }else if(itr->value().userType() == QMetaType::QString){
            item.insert("value", itr->value().toString());
        }else if(itr->value().userType() == QMetaType::Int){
            item.insert("value", itr->value().toInt());
        }else if(itr->value().userType() == QMetaType::Bool){
            item.insert("value", itr->value().toBool());
        }
        if(itr->aliasesValue().userType() == QMetaType::QStringList){
            QStringList val = itr->aliasesValue().toStringList();
            auto arrVal = QJsonArray();
            for (auto v : val) {
                arrVal.append(v);
            }
            item.insert("aliasesValue", arrVal);
        }else if(itr->aliasesValue().userType() == QMetaType::QString){
            item.insert("aliasesValue", itr->aliasesValue().toString());
        }else if(itr->aliasesValue().userType() == QMetaType::Int){
            item.insert("aliasesValue", itr->aliasesValue().toInt());
        }else if(itr->aliasesValue().userType() == QMetaType::Bool){
            item.insert("aliasesValue", itr->aliasesValue().toBool());
        }
        arr.append(item);
    }
    obj.insert("filterItems", arr);

    return obj;
}

void FilterManager::setFiltersCache(QJsonObject cache)
{
    auto itr = cache.find("loadCache");
    if(itr != cache.end())
        setloadCache(itr.value().toBool());
    itr = cache.find("nameOptions");
    if(itr != cache.end())
        setNameOptions(itr.value().toString());
    itr = cache.find("saveCache");
    if(itr != cache.end())
        setloadCache(itr.value().toBool());
    itr = cache.find("uuid");
    if(itr != cache.end())
        setUuid(itr.value().toString());

    itr = cache.find("filterItems");

    _filterItems.clear();

    if(itr != cache.end()){
        if(!itr.value().isArray())
            return;
        QJsonArray arr = itr.value().toArray();
        for (auto obj = arr.begin(); obj != arr.end(); ++obj) {
            if(!obj->isObject())
                return;
            QJsonObject _obj = obj->toObject();
            FilterItem * item = new FilterItem();
            itr = _obj.find("aliasesValue");
            if(itr != cache.end()){
                if(itr.value().isString())
                    item->setAliasesValue(itr.value().toString());
                else if (itr.value().isArray()){
                    QStringList list;
                    for(auto l : itr.value().toArray()){
                        list.append(l.toString());
                    }
                    item->setAliasesValue(list);
                }
            }
            itr = _obj.find("compareType");
            if(itr != cache.end())
                item->setCompareType((ComparisonType)itr.value().toInt());
            itr = _obj.find("key");
            if(itr != cache.end())
                item->setKey((LogEventColumn)ColumnNames.indexOf(itr.value().toString()));
            itr = _obj.find("use");
            if(itr != cache.end())
                item->setUse(itr.value().toBool());
            itr = _obj.find("value");
            if(itr != cache.end()){
                if(itr.value().isString())
                    item->setValue(itr.value().toString());
                else if (itr.value().isArray()){
                    QStringList list;
                    for(auto l : itr.value().toArray()){
                        list.append(l.toString());
                    }
                    item->setValue(list);
                }

            }

            _filterItems.insert(item->keyIndex(), item);
        }
    }
}

void FilterManager::init(const QString &uuid)
{

}

void FilterManager::setSaveCache(bool val)
{
    _saveCache = val;
}

bool FilterManager::saveCache()
{
    return _saveCache;
}

void FilterManager::setloadCache(bool val)
{
    _loadCache = val;
}

bool FilterManager::loadCache()
{
    return _loadCache;
}
