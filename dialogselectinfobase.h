#ifndef DIALOGSELECTINFOBASE_H
#define DIALOGSELECTINFOBASE_H

#include <QDialog>
#include <QMap>
#include "infobases.h"
#include <QTableWidgetItem>

namespace Ui {
class DialogSelectInfobase;
}

class DialogSelectInfobase : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSelectInfobase(QWidget *parent = nullptr);
    explicit DialogSelectInfobase(QWidget *parent, QMap<QString, Infobases*> ib);
    ~DialogSelectInfobase() override;
    QString selectedItem;



private slots:

    void on_tableInfoBase_cellDoubleClicked(int row, int column);


    void on_buttonBox_accepted();

private:
    Ui::DialogSelectInfobase *ui;
    bool isConnected;
};

#endif // DIALOGSELECTINFOBASE_H
