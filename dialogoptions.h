#ifndef DIALOGOPTIONS_H
#define DIALOGOPTIONS_H

#include <QDialog>

namespace Ui {
class DialogOptions;
}

class DialogOptions : public QDialog
{
    Q_OBJECT

public:
    explicit DialogOptions(QWidget *parent = nullptr);
    ~DialogOptions();

    QString logDbPateh;
    //bool accepted;

private slots:
    void on_btnSelectFile_clicked();

    void on_buttonBox_accepted();

    void loadAppSettings();
    void saveAppSettings();

private:
    Ui::DialogOptions *ui;


};

#endif // DIALOGOPTIONS_H
