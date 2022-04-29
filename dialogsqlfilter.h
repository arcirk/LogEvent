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


private:
    Ui::DialogSqlFilter *ui;    
    FilterManager * _filterManager;

    QStandardItemModel * m_model;
   // QMap<LogEventColumn, filterItemUi*> m_controls;

    void setControls();
    void updateData();


};

#endif // DIALOGSQLFILTER_H
