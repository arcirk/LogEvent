#include "electionlistsmodel.h"

ElectionListsModel::ElectionListsModel(QObject *parent)
    : QSqlQueryModel{parent}
{

}

Qt::ItemFlags ElectionListsModel::flags(const QModelIndex &index) const
{
    if (index.column() == 0) {
      return QSqlQueryModel::flags(index) | Qt::ItemIsUserCheckable;
    }

    return QSqlQueryModel::flags(index);
}

QVariant ElectionListsModel::data(const QModelIndex &index, int role) const
{
    if (index.column() == 0)
      {
        if (role == Qt::CheckStateRole)
      {
        int checked = QSqlQueryModel::data(index).toInt();
        if (checked)
          {
            return Qt::Checked;
          }
        else
          {
            return Qt::Unchecked;
          }
      }
        else
      {
        return QVariant();
      }
        // Only do the below if we want to fetch a tool tip.
      }
//    else if (role == Qt::ToolTipRole)
//      {
//        const QString original = QSqlQueryModel::data(index, Qt::DisplayRole).toString();
//        QString toolTip = breakString(original); // breakString() breaks the text in smaller lines.
//        return toolTip;
//      }
//    else if (role == Qt::DisplayRole)
//      {
//        const QString original = QSqlQueryModel::data(index, Qt::DisplayRole).toString();
//        QString shownText = fixBreakLines(original);
//        return shownText;
//      }
    else
      {
        return QSqlQueryModel::data(index, role);
      }
    return QVariant(); // This prevents a compiler warning.
}

bool ElectionListsModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.column() == 0 && role == Qt::CheckStateRole)
    {
      if (value == Qt::Checked)
      {
        return setData(index, 1, role);
      }
      else if (value == Qt::Unchecked)
      {
        return setData(index, 0, role);
      }
   }

    return QSqlQueryModel::setData(index, value, role);
}
