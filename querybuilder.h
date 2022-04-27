#ifndef QUERYBUILDER_H
#define QUERYBUILDER_H

#include <QSqlQueryModel>
#include <QObject>
#include <QMap>
#include <QDateTime>

enum ComparisonType{
    equals = 0,
    no_equals,
    more,
    more_or_equal,
    less_or_equal,
    less,
    on_list,
    not_in_list
};

struct BuilderPeriod{

    QDateTime startDate{};
    QDateTime endDate{};

    qint64 startdate_full_seconds(){

        qint64 sdate = startDate.toSecsSinceEpoch() + startDate.offsetFromUtc();

        return ((qint64)62135578800 + sdate) * (qint64)10000;
    }
    qint64 enddate_full_seconds(){

        qint64 edate = endDate.toSecsSinceEpoch() + endDate.offsetFromUtc();

        return ((qint64)62135578800 + edate) * (qint64)10000;
    }

};

struct FilerData{
    ComparisonType type;
    QVariant value;
    QString field;
    FilerData(){};
    ~FilerData(){};
};

class QueryBuilder : public QSqlQueryModel
{
    Q_OBJECT
public:
    explicit QueryBuilder(QObject *parent = nullptr);

    void set_period(const QDateTime& sdate, const QDateTime& edate);

    void build(QString& err);

    void addFilter(FilerData filter);

    QString toString();
private:
    BuilderPeriod period;
    QMap<QString, QString> aliases;
    QList<FilerData> m_ListFiler;
    QString getDefaultQuery();
    QString m_queryText;


};

#endif // QUERYBUILDER_H
