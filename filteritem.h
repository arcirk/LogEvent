#ifndef FILTERITEM_H
#define FILTERITEM_H

//#include <QObject>
#include "settings.h"
#include "querybuilder.h"

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

    LogEventColumn keyIndex();
    QVariant value();
    QVariant aliasesValue();
    QString key();
    ComparisonType compareType();
    bool use();

private:
    LogEventColumn _keyIndex;
    QVariant _value;
    QVariant _valueAliases;
    QString _key;
    ComparisonType _compareType;
    bool _use;

};

#endif // FILTERITEM_H
