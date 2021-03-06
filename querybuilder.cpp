#include "querybuilder.h"

QueryBuilder::QueryBuilder(QObject *parent)
    : QSqlQueryModel{parent}
{
    _isLimit = false;
    _limit = 0;
}


void QueryBuilder::set_period(const QDateTime &sdate, const QDateTime &edate)
{
    period.startDate = sdate;
    period.endDate = edate;
}

void QueryBuilder::build(bool textOnly)
{

    QString _query = getDefaultQuery();
    qint64 offset = QDateTime::currentDateTime().offsetFromUtc() - 18000;// 3*60*60;
    _query = _query.replace("offset", QString::number(offset));

    QString _where = QString("\nWHERE log.date >= %1 AND log.date <= %2").arg(QString::number(period.startdate_full_seconds()), QString::number(period.enddate_full_seconds()));

    for (auto itr = m_ListFiler.begin(); itr != m_ListFiler.end(); ++itr) {

        _where.append("\nAND ");

        if(itr->type == ComparisonType::no_equals || itr->type == ComparisonType::not_in_list){
             _where.append(" NOT ");
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

        if(itr->value.userType() == QMetaType::QString)
            _where.append(QString("'%1'").arg(itr->value.toString()));
        else if(itr->value.userType() == QMetaType::Int)
            _where.append(QString::number(itr->value.toInt()));
        else if(itr->value.userType() == QMetaType::QStringList){
            _where.append("(");
            QStringList arr = itr->value.toStringList();
            for (auto itr = arr.begin(); itr != arr.end(); ++itr) {
                _where.append(QString::fromStdString(itr->toStdString()));
                if(itr+1 != arr.end()){
                    _where.append(",");
                }
            }
            _where.append(")");

        }
        else if(itr->value.userType() == QMetaType::QDateTime) {
            qint64 val = itr->value.toDateTime().toSecsSinceEpoch() + 18000;
            _where.append(QString::number(((qint64)62135578800 + val) * (qint64)10000));
        }
    }

    if(_isLimit){
        _where.append(QString(" LIMIT %1").arg(_limit));
    }
    _query.append(_where + ";");
    m_queryText = _query;

    if(!textOnly)
        setQuery(_query);
}

void QueryBuilder::addFilter(const FilerData& filter)
{

    m_ListFiler.append(filter);

}

QString QueryBuilder::getDefaultQuery()
{
    return "SELECT CASE WHEN log.date = 0 THEN '0001-01-01 00.00.00' ELSE datetime((log.date / 10000 - 62135578800) + offset, 'unixepoch') END AS ????????,"
            "\nacodes.name AS ????????????????????,"
            "\necodes.name AS ??????????????,"
            "\npkcodes.name AS ??????????????????,"
            "\nlog.comment AS ??????????????????????,"
            "\nmdcodes.name AS ????????????????????,"
            "\nucodes.name AS ????????????????????????,"
            "\nlog.dataType AS dataType,"
            "\nlog.data AS data,"
            "\nlog.dataPresentation AS dataPresentation,"
            "\nlog.rowID AS rowID"
     "\nFROM EventLog log"
            "\nLEFT JOIN"
            "\nAppCodes acodes ON log.appCode = acodes.code"
            "\nLEFT JOIN"
            "\nEventCodes ecodes ON log.eventCode = ecodes.code"
            "\nLEFT JOIN"
            "\nComputerCodes pkcodes ON log.computerCode = pkcodes.code"
            "\nLEFT JOIN"
            "\nMetadataCodes mdcodes ON log.metadataCodes = mdcodes.code"
            "\nLEFT JOIN"
           "\nUserCodes ucodes ON log.userCode = ucodes.code";
}


QString QueryBuilder::toString() {
    return m_queryText;
}

void QueryBuilder::execQuery(const QString &query)
{
    setQuery(query);

}

void QueryBuilder::setLimit(int val)
{
    _limit = val;
    if(_limit > 0)
        _isLimit = true;
    else
        _isLimit = false;

}
