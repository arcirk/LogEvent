#ifndef SELECTIONDIALOG_H
#define SELECTIONDIALOG_H

#include <QDialog>
#include <QStandardItemModel>
#include <QStandardItem>

namespace Ui {
class SelectionDialog;
}

class SelectionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SelectionDialog(QWidget *parent, const QStringList& list, const QString& title);
    ~SelectionDialog();

    QVariant getResult() const;


private slots:

    void on_buttonBox_accepted();

    void on_listView_doubleClicked(const QModelIndex &index);

private:
    QStandardItemModel * m_model;
    Ui::SelectionDialog *ui;
    QVariant selection_result;
};

#endif // SELECTIONDIALOG_H
