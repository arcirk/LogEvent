#ifndef DIALOGSELECTCOLUMN_H
#define DIALOGSELECTCOLUMN_H

#include <QDialog>
#include <QMap>
#include "settings.h"
#include <QStandardItemModel>

namespace Ui {
class DialogSelectColumn;
}

class DialogSelectColumn : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSelectColumn(QWidget *parent = nullptr);
    explicit DialogSelectColumn(QWidget *parent, Settings * option);

    ~DialogSelectColumn() override;

private slots:
    void on_buttonBox_accepted();

private:
    Ui::DialogSelectColumn *ui;
    Settings * _option;
    QStandardItemModel * model;

};

#endif // DIALOGSELECTCOLUMN_H
