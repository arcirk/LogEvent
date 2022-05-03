#ifndef DIALOGSVALEFILTER_H
#define DIALOGSVALEFILTER_H

#include <QDialog>
#include <filtermanager.h>
#include <QStandardItemModel>
#include <QTableWidgetItem>

namespace Ui {
class DialogSaveFilter;
}

class DialogSaveFilter : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSaveFilter(FilterManager * manager, bool SelectedMode, QWidget *parent = nullptr);
    ~DialogSaveFilter();

    void accept() override;

    QUuid getResult();

private slots:
    void on_btnAdd_clicked();

    void on_btnDelete_clicked();

    void on_btnCopy_clicked();

    void on_btnSelect_clicked();

    void on_tableWidget_doubleClicked(const QModelIndex &index);

    void onCheckBoxToggled(bool value);

    void on_buttonBox_accepted();

private:
    Ui::DialogSaveFilter *ui;
    bool _selectedMode;
    FilterManager * _manager;
    int currentRowLoad;
    QUuid _result;

    void init_model();

    void add_filter_item(filter_options * _opt = nullptr);

    void set_default_item();

};

#endif // DIALOGSVALEFILTER_H
