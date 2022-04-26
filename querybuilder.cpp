#include "querybuilder.h"

QueryBuilder::QueryBuilder(QObject *parent)
    : QSqlQueryModel{parent}
{

}

void QueryBuilder::set_period(const QDateTime &sdate, const QDateTime &edate)
{
    period.startDate = sdate;
    period.endDate = edate;
}

void QueryBuilder::build(QString& err)
{

    qint64 startDate = period.startdate_full_seconds();
    qint64 endDate = period.enddate_full_seconds();

    if(startDate > endDate){
        err = "Дата начала выборки не может быть больше даты окончания!";
        return;
    }

    QString _query = getDefaultQuery();
    QString _where = "\nWHERE";

    for (auto itr = m_ListFiler.begin(); itr != m_ListFiler.end(); ++itr) {
        if(itr != m_ListFiler.begin()){
            _where.append("\nAND");
        }else
            _where.append("\n");

        if(itr->type == ComparisonType::no_equals || itr->type == ComparisonType::not_in_list){
             _where.append("NOT ");
        }
        _where.append("log." + itr->field);

        if(itr->type == ComparisonType::equals || itr->type == ComparisonType::no_equals)
            _where.append(" = ");
        else if(itr->type == ComparisonType::not_in_list || itr->type == ComparisonType::on_list)
           _where.append(" IN ");
        else if(itr->type == ComparisonType::less)
            _where.append(" < ");
        else if(itr->type == ComparisonType::more)
            _where.append(" > ");
        else if(itr->type == ComparisonType::less_or_equal)
            _where.append(" <= ");
        else if(itr->type == ComparisonType::more_or_equal)
            _where.append(" >= ");

        QVariant value = itr->value;
        QString sz_value;
        if(value.userType() == QMetaType::QString)
            sz_value = QString("'%1'").arg(value.toString());
        else if(value.userType() == QMetaType::Int)
            sz_value = QString::number(value.toInt());
        else if(value.userType() == QMetaType::QStringList){
            sz_value = "(";
            QStringList arr = value.toStringList();
            for (auto itr = arr.begin(); itr != arr.end(); ++itr) {
                sz_value.append(QString::fromStdString(itr->toStdString()));
                if(itr+1 != arr.end()){
                    _where.append(",");
                }
            }
            sz_value.append(")");
        }
    }

}

void QueryBuilder::addFilter(FilerData filter)
{

    m_ListFiler.append(filter);

}

QString QueryBuilder::getDefaultQuery()
{
    return "SELECT CASE WHEN log.date = 0 THEN '0001-01-01 00.00.00' ELSE datetime(log.date / 10000 - 62135578800, 'unixepoch') END AS date,"
            "acodes.name AS appName,"
            "ecodes.name AS event,"
            "pkcodes.name AS computer,"
            "log.comment AS comment,"
            "mdcodes.name AS metadata,"
            "ucodes.name AS user,"
            "log.dataType AS dataType,"
            "log.data AS data,"
            "log.dataPresentation AS dataPresentation,"
            "log.rowID AS rowID"
     "FROM EventLog log"
            "LEFT JOIN"
            "AppCodes acodes ON log.appCode = acodes.code"
            "LEFT JOIN"
            "EventCodes ecodes ON log.eventCode = ecodes.code"
            "LEFT JOIN"
            "ComputerCodes pkcodes ON log.computerCode = pkcodes.code"
            "LEFT JOIN"
            "MetadataCodes mdcodes ON log.metadataCodes = mdcodes.code"
            "LEFT JOIN"
            "UserCodes ucodes ON log.userCode = ucodes.code";
}
