#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QCoreApplication>
#include <QStringList>
#include <QDebug>
#include <QSettings>
#define UPdateTime 2  // in seconds


/*
#define SITE_1 "http://192.168.1.2/"
#define SITE_2 "http://192.168.1.3/"
#define SITE_3 "http://192.168.1.4/"
*/

QString SITE_1 = "http://192.168.3.5/";
//QString SITE_1 "http://192.168.106.1:8000/first";
QString SITE_2 = "http://192.168.106.1:8000/sec";
QString SITE_3 = "http://192.168.106.1:8000/third";


QList<QString> MycolorList;

float savelevel=30;
float waterlevel=0;
float temperature=0;
unsigned char out_stats=0;

QString siteaddr;
QTimer *mytimer;
QNetworkReply *reply;
QNetworkAccessManager *manager;

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    ui->comboBox_2->setCurrentIndex(0);
    loadsettings2();

    ui->lineEdit->installEventFilter(this);
    ui->lineEdit_2->installEventFilter(this);
    ui->lineEdit_3->installEventFilter(this);
    ui->lineEdit_4->installEventFilter(this);
    ui->lineEdit_5->installEventFilter(this);
    ui->lineEdit_6->installEventFilter(this);


    //ui->frame_1->setHidden(true);
    //ui->frame_1->setDisabled(true);
    ui->lcdNumber->display("00.00 'C");
    ui->textEdit->setReadOnly(true);
    ui->comboBox->setCurrentIndex(0);

    loadsettings();

    setWindowTitle(tr("PTN Remote Controller")); // window title


    mytimer= new QTimer(this);
    mytimer->setInterval(UPdateTime*1000);
    mytimer->start();
    connect(mytimer,SIGNAL(timeout()),this,SLOT(updatetime()));


}

MainWindow::~MainWindow()
{
    delete ui;


}

void MainWindow::on_comboBox_activated(int index)
{

    loadsettings();


}





void MainWindow::loadsettings()
{

    QSettings settings(QSettings::IniFormat, QSettings::UserScope,"mycompany","myapp",this);
    settings.beginGroup("Sites");

    ui->comboBox->setItemText(0,settings.value("site1","site1").toString());
    ui->comboBox->setItemText(1,settings.value("site2","site2").toString());
    ui->comboBox->setItemText(2,settings.value("site3","site3").toString());

    settings.endGroup();


    switch (ui->comboBox->currentIndex()){

    case 0:
        siteaddr=SITE_1;
        settings.beginGroup("Sites1_out");
        ui->out1->setText(settings.value("out1","out1").toString());
        ui->out2->setText(settings.value("out2","out2").toString());
        ui->out3->setText(settings.value("out3","out3").toString());
        ui->frame_1->setEnabled(settings.value("out1_status",1).toBool());
        ui->frame_2->setEnabled(settings.value("out2_status",1).toBool());
        ui->frame_3->setEnabled(settings.value("out3_status",1).toBool());
        settings.endGroup();
        break;

    case 1:
        siteaddr=SITE_2;
        settings.beginGroup("Sites2_out");
        ui->out1->setText(settings.value("out1","out1").toString());
        ui->out2->setText(settings.value("out2","out2").toString());
        ui->out3->setText(settings.value("out3","out3").toString());
        ui->frame_1->setEnabled(settings.value("out1_status",1).toBool());
        ui->frame_2->setEnabled(settings.value("out2_status",1).toBool());
        ui->frame_3->setEnabled(settings.value("out3_status",1).toBool());
        settings.endGroup();
        break;

    case 2:
        siteaddr=SITE_3;
        settings.beginGroup("Sites3_out");
        ui->out1->setText(settings.value("out1","out1").toString());
        ui->out2->setText(settings.value("out2","out2").toString());
        ui->out3->setText(settings.value("out3","out3").toString());
        ui->frame_1->setEnabled(settings.value("out1_status",1).toBool());
        ui->frame_2->setEnabled(settings.value("out2_status",1).toBool());
        ui->frame_3->setEnabled(settings.value("out3_status",1).toBool());
        settings.endGroup();
        break;


    }





}



void MainWindow::updatetime(){
    // ui->timelabel->setText(QTime::currentTime().toString("h:mm:ss "));
    sendRequest( siteaddr);

    if (waterlevel > savelevel){
        ui->textEdit->setStyleSheet("background-color: red;");
    }
    else
        ui->textEdit->setStyleSheet("background-color: white;");

}


void  MainWindow::sendRequest( QString url){
    QTimer tT;

    manager =new QNetworkAccessManager(this);

    // create custom temporary event loop on stack
    QEventLoop eventLoop;
    tT.setSingleShot(true);

    // "quit()" the event-loop, when the network request "finished()"
    QObject::connect(&tT, SIGNAL(timeout()), &eventLoop, SLOT(quit()));

    QObject::connect(manager, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));


    tT.start(500); // 500 milli-seconds timeout
    reply = manager->get(QNetworkRequest( url ));
    eventLoop.exec(); // blocks stack until "finished()" has been called


    if(tT.isActive()){




        // download complete
        tT.stop();

        if (ui->frame_1->isHidden()){
            ui->error_label->setText("");
            ui->frame_1->setHidden(false);
            ui->frame_2->setHidden(false);
            ui->frame_3->setHidden(false);
        }


        if (reply->error() == QNetworkReply::NoError ) {
            //success

            QString strReply = reply->readAll();
            if (strReply !="")
            {

                if (url == siteaddr){
                    MycolorList=strReply.split(",");
                    waterlevel=MycolorList[0].toFloat();
                    temperature=MycolorList[1].toFloat();
                    out_stats=MycolorList[2].toInt();
                    ui->textEdit->setText(MycolorList[0]);
                    ui->lcdNumber->display(MycolorList[1] + " 'C");
                }

                else {
                    out_stats= strReply.toInt();
                }

                showoutstas();
            }
        }
    }

    else {
        if (!ui->frame_1->isHidden()){

            ui->frame_1->setHidden(true);
            ui->frame_2->setHidden(true);
            ui->frame_3->setHidden(true);
            ui->error_label->setText("Failure !! check the LAN connection ");

        }


        //failure
        //ui->statusBar->showMessage("Failure !! check the LAN connection of <h1>"+ui->comboBox->currentText()+
        //                           "</h1>  Cause: "+reply->errorString(),2000);



    }


    delete reply;
    delete manager;

}



void MainWindow::on_out1_on_clicked()
{
    mytimer->stop();
    sendRequest( siteaddr + QString("?out1=1"));
    mytimer->start();
}

void MainWindow::on_out1_off_clicked()
{
    mytimer->stop();
    sendRequest( siteaddr + QString("?out1=0"));
    mytimer->start();

}

void MainWindow::on_out2_on_clicked()
{
    mytimer->stop();
    sendRequest( siteaddr + QString("?out2=1"));
    mytimer->start();

}

void MainWindow::on_out2_off_clicked()
{
    mytimer->stop();
    sendRequest( siteaddr + QString("?out2=0"));
    mytimer->start();

}

void MainWindow::on_out3_on_clicked()
{
    mytimer->stop();
    sendRequest( siteaddr + QString("?out3=1"));
    mytimer->start();

}

void MainWindow::on_out3_off_clicked()
{
    mytimer->stop();
    sendRequest( siteaddr + QString("?out3=0"));
    mytimer->start();

}


void MainWindow::showoutstas(){

    if (ui->frame_1->isEnabled()){

        if (out_stats & 0x01){
            ui->out1_on_label->setPixmap(QPixmap(":/images/red.png"));
            ui->out1_off_label->setPixmap(QPixmap(":/images/black.png"));

        }

        else{
            ui->out1_on_label->setPixmap(QPixmap(":/images/black.png"));
            ui->out1_off_label->setPixmap(QPixmap(":/images/red.png"));


        }
    }

    if (ui->frame_2->isEnabled()){

        if (out_stats & 0x02){
            ui->out2_on_label->setPixmap(QPixmap(":/images/red.png"));
            ui->out2_off_label->setPixmap(QPixmap(":/images/black.png"));

        }

        else{
            ui->out2_on_label->setPixmap(QPixmap(":/images/black.png"));
            ui->out2_off_label->setPixmap(QPixmap(":/images/red.png"));


        }

    }


    if (ui->frame_3->isEnabled()){

        if (out_stats & 0x04){
            ui->out3_on_label->setPixmap(QPixmap(":/images/red.png"));
            ui->out3_off_label->setPixmap(QPixmap(":/images/black.png"));

        }

        else{
            ui->out3_on_label->setPixmap(QPixmap(":/images/black.png"));
            ui->out3_off_label->setPixmap(QPixmap(":/images/red.png"));

        }

    }


}



void MainWindow::on_savebuttons_clicked()
{

    QSettings settings(QSettings::IniFormat, QSettings::UserScope,"mycompany","myapp",this);
    settings.beginGroup("Sites");

    if (!ui->lineEdit->text().isEmpty())
        settings.setValue("site1",ui->lineEdit->text());
    if (!ui->lineEdit_2->text().isEmpty())
        settings.setValue("site2",ui->lineEdit_2->text());
    if (!ui->lineEdit_3->text().isEmpty())
        settings.setValue("site3",ui->lineEdit_3->text());
    settings.endGroup();


    switch (ui->comboBox_2->currentIndex()){
    case 0:
        settings.beginGroup("Sites1_out");
        if (!ui->lineEdit_4->text().isEmpty())
            settings.setValue("out1",ui->lineEdit_4->text());
        if (!ui->lineEdit_5->text().isEmpty())
            settings.setValue("out2",ui->lineEdit_5->text());
        if (!ui->lineEdit_6->text().isEmpty())
            settings.setValue("out3",ui->lineEdit_6->text());
        settings.setValue("out1_status",ui->checkBox1->isChecked()?1:0);
        settings.setValue("out2_status",ui->checkBox2->isChecked()?1:0);
        settings.setValue("out3_status",ui->checkBox3->isChecked()?1:0);
        settings.endGroup();
        break;

    case 1:
        settings.beginGroup("Sites2_out");
        if (!ui->lineEdit_4->text().isEmpty())
            settings.setValue("out1",ui->lineEdit_4->text());
        if (!ui->lineEdit_5->text().isEmpty())
            settings.setValue("out2",ui->lineEdit_5->text());
        if (!ui->lineEdit_6->text().isEmpty())
            settings.setValue("out3",ui->lineEdit_6->text());
        settings.setValue("out1_status",ui->checkBox1->isChecked()?1:0);
        settings.setValue("out2_status",ui->checkBox2->isChecked()?1:0);
        settings.setValue("out3_status",ui->checkBox3->isChecked()?1:0);
        settings.endGroup();
        break;

    case 2:
        settings.beginGroup("Sites3_out");
        if (!ui->lineEdit_4->text().isEmpty())
            settings.setValue("out1",ui->lineEdit_4->text());
        if (!ui->lineEdit_5->text().isEmpty())
            settings.setValue("out2",ui->lineEdit_5->text());
        if (!ui->lineEdit_6->text().isEmpty())
            settings.setValue("out3",ui->lineEdit_6->text());
        settings.setValue("out1_status",ui->checkBox1->isChecked()?1:0);
        settings.setValue("out2_status",ui->checkBox2->isChecked()?1:0);
        settings.setValue("out3_status",ui->checkBox3->isChecked()?1:0);
        settings.endGroup();
        break;


    }

}

void MainWindow::on_comboBox_2_activated(int index)
{
    loadsettings2();
}




void MainWindow::loadsettings2()
{

    QSettings settings(QSettings::IniFormat, QSettings::UserScope,"mycompany","myapp",this);
    settings.beginGroup("Sites");

    ui->lineEdit->setText(settings.value("site1","site1").toString());
    ui->lineEdit_2->setText(settings.value("site2","site2").toString());
    ui->lineEdit_3->setText(settings.value("site3","site3").toString());

    settings.endGroup();


    switch (ui->comboBox_2->currentIndex()){

    case 0:
        settings.beginGroup("Sites1_out");
        ui->lineEdit_4->setText(settings.value("out1","out1").toString());
        ui->lineEdit_5->setText(settings.value("out2","out2").toString());
        ui->lineEdit_6->setText(settings.value("out3","out3").toString());
        ui->checkBox1->setChecked(settings.value("out1_status",1).toBool());
        ui->checkBox2->setChecked(settings.value("out2_status",1).toBool());
        ui->checkBox3->setChecked(settings.value("out3_status",1).toBool());
        settings.endGroup();
        break;

    case 1:
        settings.beginGroup("Sites2_out");
        ui->lineEdit_4->setText(settings.value("out1","out1").toString());
        ui->lineEdit_5->setText(settings.value("out2","out2").toString());
        ui->lineEdit_6->setText(settings.value("out3","out3").toString());
        ui->checkBox1->setChecked(settings.value("out1_status",1).toBool());
        ui->checkBox2->setChecked(settings.value("out2_status",1).toBool());
        ui->checkBox3->setChecked(settings.value("out3_status",1).toBool());
        settings.endGroup();
        break;

    case 2:
        settings.beginGroup("Sites3_out");
        ui->lineEdit_4->setText(settings.value("out1","out1").toString());
        ui->lineEdit_5->setText(settings.value("out2","out2").toString());
        ui->lineEdit_6->setText(settings.value("out3","out3").toString());
        ui->checkBox1->setChecked(settings.value("out1_status",1).toBool());
        ui->checkBox2->setChecked(settings.value("out2_status",1).toBool());
        ui->checkBox3->setChecked(settings.value("out3_status",1).toBool());
        settings.endGroup();
        break;


    }


}



bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if(event->type() == QEvent::FocusIn) {
        QTimer::singleShot(0, obj, SLOT(selectAll()));
        return false;
    } else {
        return QObject::eventFilter(obj, event);
    }
}





