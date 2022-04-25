#ifndef DIALOGSELECTCOLUMN_H
#define DIALOGSELECTCOLUMN_H

#include <QDialog>
#include <QMap>

namespace Ui {
class DialogSelectColumn;
}

class DialogSelectColumn : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSelectColumn(QWidget *parent = nullptr);
    explicit DialogSelectColumn(QWidget *parent, QMap<QString,bool> &option);

    ~DialogSelectColumn();

private:
    Ui::DialogSelectColumn *ui;
};

#endif // DIALOGSELECTCOLUMN_H
