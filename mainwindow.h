#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QTimer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void showoutstas();
    QPixmap colorSwatch( const QColor color );

private:
    Ui::MainWindow *ui;


private slots:
    void on_out3_off_clicked();
    void on_out3_on_clicked();
    void on_out2_off_clicked();
    void on_out2_on_clicked();
    void on_out1_off_clicked();
    void on_out1_on_clicked();
    void on_settingsbut_clicked();
    void on_comboBox_activated(int index);
    void loadsettings();
    void updatetime();
    void sendRequest(QString);
};

#endif // MAINWINDOW_H
