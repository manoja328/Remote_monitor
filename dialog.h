#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QSettings>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
    bool eventFilter(QObject *obj, QEvent *event);

private:
    Ui::Dialog *ui;
    void loadsettings();

private slots:
    void on_comboBox_activated(int index);
    void on_pushButton_clicked();

};

#endif // DIALOG_H
