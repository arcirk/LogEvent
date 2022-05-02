#ifndef FILTERITEM_H
#define FILTERITEM_H

//#include <QObject>
#include "settings.h"
#include "querybuilder.h"
#include <QUuid>

class FilterItem
{
    //Q_OBJECT
public:

    explicit FilterItem();
    ~FilterItem(){};

    void setKey(LogEventColumn index);
    void setValue(const QVariant& val);
    void setAliasesValue(const QVariant& val);
    void setCompareType(ComparisonType type);
    void setUse(bool val);
    void setUuid(const QUuid& val);

    LogEventColumn keyIndex();
    QVariant value();
    QVariant aliasesValue();
    QString key();
    ComparisonType compareType();
    bool use();
    QUuid uuid();

private:
    LogEventColumn _keyIndex;
    QVariant _value;
    QVariant _valueAliases;
    QString _key;
    ComparisonType _compareType;
    bool _use;
    QUuid _uuid;

};

#endif // FILTERITEM_H
