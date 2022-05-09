#ifndef DIALOGSELECTITEMSFILTER_H
#define DIALOGSELECTITEMSFILTER_H

#include <QDialog>
#include <QSqlDatabase>
#include <QStandardItemModel>
#include "querybuilder.h"

namespace Ui {
class DialogSelectItemsFilter;
}

class DialogSelectItemsFilter : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSelectItemsFilter(QWidget *parent = nullptr);
    explicit DialogSelectItemsFilter(QWidget *parent , const QString& table, const QString& tableAlias, const QStringList& lst, ComparisonType compareType);
    ~DialogSelectItemsFilter() override;

    QVariant selectedValue;
    QVariant selectedCode;

private slots:
    void on_btnUncheckedAll_clicked();

    void on_btnCheckedAll_clicked();

    void on_tableView_doubleClicked(const QModelIndex &index);

    void on_buttonBox_accepted();

private:
    Ui::DialogSelectItemsFilter *ui;
    QStandardItemModel * model;
    bool isList;


    void checked_unchecked(Qt::CheckState value);
    void init_sql_table(const QString& table, const QStringList& lst);
    void init_table(const QStringList& lst);

};

#endif // DIALOGSELECTITEMSFILTER_H
