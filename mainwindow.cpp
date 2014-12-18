#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QStringList>
#include "dialog.h"
#include <QPainter>
#define UPdateTime 2  // in seconds


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
    setWindowTitle(tr("PTN Remote Controller")); // window title

    /*
    mydialog =new Dialog(this);
    mydialog->setModal(true);
    mydialog->setWindowModality((Qt::WindowModal));
*/

    //ui->frame_1->setHidden(true);
    //ui->frame_1->setDisabled(true);
    ui->lcdNumber->display("00.00 'C");
    ui->textEdit->setReadOnly(true);
    ui->comboBox->setCurrentIndex(0);

    loadsettings();


    ui->loading1->hide();
    ui->loading2->hide();
    ui->loading3->hide();

    mytimer= new QTimer(this);
    mytimer->setInterval(UPdateTime*1000);
    mytimer->start();
    connect(mytimer,SIGNAL(timeout()),this,SLOT(updatetime()));


}

MainWindow::~MainWindow()
{
    delete ui;
}


QPixmap MainWindow::colorSwatch( const QColor color )
{
    QPixmap pixmap(20, 20 );
    pixmap.fill(Qt::white);
    QPainter painter;
    painter.begin( &pixmap );
    painter.setBrush( color );
    painter.drawEllipse( 0, 0,15,15 );
    painter.end();
    return pixmap;
}



void MainWindow::on_comboBox_activated(int index)
{
    loadsettings();

}


void MainWindow::on_settingsbut_clicked()
{

    Dialog *mydialog=new Dialog(this);
    mydialog->setModal(true);
    mydialog->setWindowModality((Qt::WindowModal));
    mydialog->exec();
    delete mydialog;
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

    ui->label->setText(ui->comboBox->currentText());



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
            ui->error_label->setText("Failure !! check the LAN connection");

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
    ui->loading1->show();
    sendRequest( siteaddr + QString("?out1=1"));
    ui->loading1->hide();
    mytimer->start();
}

void MainWindow::on_out1_off_clicked()
{
    mytimer->stop();
    ui->loading1->show();
    sendRequest( siteaddr + QString("?out1=0"));
    ui->loading1->hide();
    mytimer->start();

}

void MainWindow::on_out2_on_clicked()
{
    mytimer->stop();
    ui->loading2->show();
    sendRequest( siteaddr + QString("?out2=1"));
    ui->loading2->hide();
    mytimer->start();

}

void MainWindow::on_out2_off_clicked()
{
    mytimer->stop();
    ui->loading2->show();
    sendRequest( siteaddr + QString("?out2=0"));
    ui->loading2->hide();
    mytimer->start();

}

void MainWindow::on_out3_on_clicked()
{
    mytimer->stop();
    ui->loading3->show();
    sendRequest( siteaddr + QString("?out3=1"));
    ui->loading3->hide();
    mytimer->start();

}

void MainWindow::on_out3_off_clicked()
{
    mytimer->stop();
    ui->loading3->show();
    sendRequest( siteaddr + QString("?out3=0"));
    ui->loading3->hide();
    mytimer->start();

}


void MainWindow::showoutstas(){

    if (ui->frame_1->isEnabled()){

        if (out_stats & 0x01){
            ui->out1_on_label->setPixmap(colorSwatch(Qt::green));
            ui->out1_off_label->setPixmap(colorSwatch(Qt::black));

        }

        else{
            ui->out1_on_label->setPixmap(colorSwatch(Qt::black));
            ui->out1_off_label->setPixmap(colorSwatch(Qt::green));


        }
    }

    if (ui->frame_2->isEnabled()){

        if (out_stats & 0x02){
            ui->out2_on_label->setPixmap(colorSwatch(Qt::green));
            ui->out2_off_label->setPixmap(colorSwatch(Qt::black));

        }

        else{
            ui->out2_on_label->setPixmap(colorSwatch(Qt::black));
            ui->out2_off_label->setPixmap(colorSwatch(Qt::green));


        }

    }


    if (ui->frame_3->isEnabled()){

        if (out_stats & 0x04){
            ui->out3_on_label->setPixmap(colorSwatch(Qt::green));
            ui->out3_off_label->setPixmap(colorSwatch(Qt::black));

        }

        else{
            ui->out3_on_label->setPixmap(colorSwatch(Qt::black));
            ui->out3_off_label->setPixmap(colorSwatch(Qt::green));

        }

    }


}
