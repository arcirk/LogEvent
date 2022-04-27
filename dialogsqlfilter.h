#ifndef DIALOGSQLFILTER_H
#define DIALOGSQLFILTER_H

#include <QDialog>
#include <QSqlDatabase>

namespace Ui {
class DialogSqlFilter;
}

class DialogSqlFilter : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSqlFilter(QWidget *parent, QSqlDatabase& db, const QString& saveOptions = "");
    ~DialogSqlFilter();

private slots:
    void on_chSelApp_toggled(bool checked);

    void on_btnSelApp_clicked();

private:
    Ui::DialogSqlFilter *ui;
    QSqlDatabase& m_db;
    QString _save_options;
};

#endif // DIALOGSQLFILTER_H
