#ifndef DIALOGSQLFILTER_H
#define DIALOGSQLFILTER_H

#include <QDialog>
#include <QSqlDatabase>
#include "filtermanager.h"
#include "filteritem.h"
#include "settings.h"
#include <QMap>
#include <QCheckBox>
#include <QLineEdit>
#include <QStandardItemModel>
#include <QUuid>

namespace Ui {
class DialogSqlFilter;
}

class DialogSqlFilter : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSqlFilter(QWidget *parent, FilterManager * filterManager);
    ~DialogSqlFilter();

private slots:

    void onToolButtonToggle();

    void on_btnFilterItemDelete_clicked();

    void on_tableWidget_doubleClicked(const QModelIndex &index);

    void onSelectedFormShow(ComparisonType type , QWidget *pWidgetVal, const QString& field, int row);

    void onItemComboCurrentIndexChanged(int index);

    void on_buttonBox_accepted();

    void on_btnFilterItemAdd_clicked();

    void on_btnSaveFilter_clicked();

    void on_btnLoadFilter_clicked();

    void onResetFilter();

    void onDateTimeChanged(const QDateTime &dateTime);

private:
    Ui::DialogSqlFilter *ui;    
    FilterManager * _filterManager;

    QStandardItemModel * m_model;

    void addFilter(const QString& filter_field, const QUuid& uuid = QUuid::createUuid());
    void updateManager();

    QMap<QString, ComparisonType> _compareType;
    QMap<QString, QString> _aliases;
    QMap<QString, QString> _aliasesField;

    const QStringList szComparisonType{
        "равно",
        "не равно",
        "больше",
        "больше или равно",
        "меньше",
        "меньше или равно",
        "в списке",
        "не в списке"
    };

    void loadFilterItems();

signals:

    void selectedFormShow(ComparisonType type , QWidget *pWidgetVal, const QString& field, int row);
    void itemComboCurrentIndexChanged(int index);

};

#endif // DIALOGSQLFILTER_H
