#ifndef DIALOGSELECTEDROW_H
#define DIALOGSELECTEDROW_H

#include <QDialog>
#include <QMap>

namespace Ui {
class DialogSelectedRow;
}

class DialogSelectedRow : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSelectedRow(QMap<QString, QString> row, QWidget *parent = nullptr);
    ~DialogSelectedRow() override;

private:
    Ui::DialogSelectedRow *ui;
};

#endif // DIALOGSELECTEDROW_H
