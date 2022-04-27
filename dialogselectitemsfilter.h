#ifndef DIALOGSELECTITEMSFILTER_H
#define DIALOGSELECTITEMSFILTER_H

#include <QDialog>
#include <QSqlDatabase>
#include <QStandardItemModel>

namespace Ui {
class DialogSelectItemsFilter;
}

class DialogSelectItemsFilter : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSelectItemsFilter(QWidget *parent = nullptr);
    explicit DialogSelectItemsFilter(QWidget *parent , const QString& table, QSqlDatabase& db, const QString& saveOptions = "");
    ~DialogSelectItemsFilter();

private slots:
    void on_btnUncheckedAll_clicked();

    void on_btnCheckedAll_clicked();

private:
    Ui::DialogSelectItemsFilter *ui;
    QStandardItemModel * model;
    void checked_unchecked(Qt::CheckState value);
};

#endif // DIALOGSELECTITEMSFILTER_H
