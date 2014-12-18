#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QCoreApplication>
#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QStringList>
#include <QTimer>
#include <QTime>
#include <phonon/mediaobject.h>
#include <phonon/audiooutput.h>
#define UPdateTime 1  // in seconds



Phonon::MediaObject* mMediaObject;
Phonon::AudioOutput* mAudioOutput;
// to play music give the full path to the source not after the resoource name


unsigned long time=01;
QList<QString> MycolorList;
float savelevel=300;

float waterlevel=0;
float temperature=0;


MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle(tr("Weather Station")); // window title


    mMediaObject = new Phonon::MediaObject(this);
    mAudioOutput = new Phonon::AudioOutput(Phonon::MusicCategory, this);
    mAudioOutput->setVolume(0.5);
    Phonon::createPath(mMediaObject, mAudioOutput);
    mMediaObject->setCurrentSource(QString("C:/Users/Reddragon/Desktop/ptn_remote/ping.wav"));

    QTimer *mytimer= new QTimer(this);
    mytimer->setInterval(UPdateTime*1000);
    mytimer->start();
    connect(mytimer,SIGNAL(timeout()),this,SLOT(updatetime()));
    ui->lcdNumber->display("00:00");
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::updatetime(){
    ui->timelabel->setText(QTime::currentTime().toString("h:mm:ss "));

    ui->timelabel->setText(QTime::currentTime().toString("h:mm:ss "));


    sendRequest( QString("http://192.168.3.5/rgb"));
    QTime displayTime(0, time / 60, time % 60,0);

    ui->lcdNumber->display(displayTime.toString("mm:ss"));
    time++;

    if (waterlevel > savelevel){
        ui->textEdit->setStyleSheet("background-color: red;");
        mMediaObject->play();
        mMediaObject->seek(0);
    }
    else
        ui->textEdit->setStyleSheet("background-color: white;");


}


void MainWindow::on_pushButton_clicked()
{

    //sendRequest( QString("http://192.168.3.5/rgb"));

    //better do this next tiem sendrequest("ardhino/open=1")
}


void  MainWindow::sendRequest( QString a){

    // create custom temporary event loop on stack
    QEventLoop eventLoop;

    // "quit()" the event-loop, when the network request "finished()"
    QNetworkAccessManager mgr;
    QObject::connect(&mgr, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));

    // the HTTP request
    // QNetworkRequest req( QUrl( QString("http://ip.jsontest.com/") ) );
    QNetworkRequest req( a );
    QNetworkReply *reply = mgr.get(req);
    eventLoop.exec(); // blocks stack until "finished()" has been called

    if (reply->error() == QNetworkReply::NoError && a=="http://192.168.3.5/rgb") {
        //success

        QString strReply = (QString)reply->readAll();
        qDebug() << "Success" <<strReply;
        strReply.remove("}");
        MycolorList=strReply.split(",");

        waterlevel=QString(MycolorList[0].split(":")[1]).toFloat();
        temperature=QString(MycolorList[1].split(":")[1]).toFloat();
        qDebug() << waterlevel;
        qDebug() << temperature;
        ui->textEdit->setText(MycolorList[0].split(":")[1]);
        ui->textEdit_2->setText(MycolorList[1].split(":")[1]);
        delete reply;
    }
    else {
        //failure
        qDebug() << "Failure check your LAN" <<reply->errorString();
        delete reply;
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    qDebug() << "off button clicked";

}


void MainWindow::on_out2_on_clicked()
{
    sendRequest( QString("http://192.168.3.5/closegate=1"));
    ui->label_9->setPixmap(QPixmap(":/images/b.png"));
    ui->label_10->setPixmap(QPixmap(":/images/a.png"));
    //input this in browser   http://192.168.3.5/closegate=1

    qDebug() << ui->out2_on->objectName() << "was opened";


}

void MainWindow::on_out2_off_clicked()
{
    sendRequest( QString("http://192.168.3.5/closegate=0"));
    ui->label_9->setPixmap(QPixmap(":/images/a.png"));
    ui->label_10->setPixmap(QPixmap(":/images/b.png"));

    qDebug() << ui->out2_off->objectName() << "was closed";
}

void MainWindow::on_out3_on_clicked()
{

    sendRequest( QString("http://192.168.3.5/closemotor=1"));
}

void MainWindow::on_out3_off_clicked()
{
    sendRequest( QString("http://192.168.3.5/closemotor=0"));
}
