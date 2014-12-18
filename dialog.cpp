#include "dialog.h"
#include "ui_dialog.h"
#include <QTimer>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{

    ui->setupUi(this);
    setWindowTitle(tr("Settings Window")); // window title
    ui->comboBox->setCurrentIndex(0);
    loadsettings();

    ui->lineEdit->installEventFilter(this);
    ui->lineEdit_2->installEventFilter(this);
    ui->lineEdit_3->installEventFilter(this);
    ui->lineEdit_4->installEventFilter(this);
    ui->lineEdit_5->installEventFilter(this);
    ui->lineEdit_6->installEventFilter(this);

}

Dialog::~Dialog()
{
    delete ui;

}



bool Dialog::eventFilter(QObject *obj, QEvent *event)
{
    if(event->type() == QEvent::FocusIn) {
        QTimer::singleShot(0, obj, SLOT(selectAll()));
        return false;
    } else {
        return QObject::eventFilter(obj, event);
    }
}



void Dialog::on_pushButton_clicked()
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


    switch (ui->comboBox->currentIndex()){
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



void Dialog::loadsettings()
{


    QSettings settings(QSettings::IniFormat, QSettings::UserScope,"mycompany","myapp",this);
    settings.beginGroup("Sites");

    ui->lineEdit->setText(settings.value("site1","site1").toString());
    ui->lineEdit_2->setText(settings.value("site2","site2").toString());
    ui->lineEdit_3->setText(settings.value("site3","site3").toString());

    settings.endGroup();


    switch (ui->comboBox->currentIndex()){

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


void Dialog::on_comboBox_activated(int index)
{
    loadsettings();
}
