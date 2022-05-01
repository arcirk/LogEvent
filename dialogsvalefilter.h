#ifndef DIALOGSVALEFILTER_H
#define DIALOGSVALEFILTER_H

#include <QDialog>
#include <filtermanager.h>
#include <QStandardItemModel>
#include <QTableWidgetItem>

namespace Ui {
class DialogSvaleFilter;
}

class DialogSvaleFilter : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSvaleFilter(FilterManager * manager, bool SelectedMode, QWidget *parent = nullptr);
    ~DialogSvaleFilter();

    void accept() override;

private slots:
    void on_btnAdd_clicked();

    void on_btnDelete_clicked();

private:
    Ui::DialogSvaleFilter *ui;
    bool _selectedMode;
    FilterManager * _manager;

    void init_model();

    void add_filter_item(filter_options * _opt = nullptr);

};

#endif // DIALOGSVALEFILTER_H
