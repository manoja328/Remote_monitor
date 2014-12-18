#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore>
namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void sendRequest( QString a);


private:
    Ui::MainWindow *ui;

private slots:
    void on_out3_off_clicked();
    void on_out3_on_clicked();
    void on_out2_off_clicked();
    void on_out2_on_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_clicked();
    void updatetime();

};





#endif // MAINWINDOW_H
