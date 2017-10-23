#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtSerialPort/QSerialPortInfo>



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //查找串口
//    ui->combox->setDisabled(true);
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
       {
           QSerialPort serial;
           serial.setPort(info);
           if(serial.open(QIODevice::ReadWrite))
           {
               ui->combox->addItem(serial.portName());
               serial.close();
           }
       }
  // connect(ui->openqsort,SIGNAL(clicked(),this,SLOT(on_openqsort_clicked()));
  // connect(ui->closeqsort,SIGNAL(clicked()),this,SLOT(on_closeqsort_clicked()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_stop_clicked()
{

}

void MainWindow::on_run_clicked()
{

}

void MainWindow::on_openqsort_clicked()
{
    ui->combox->setEnabled(false);
//    ui->combox->setEditable(false);
    serial = new QSerialPort;

    //set com
    serial->setPortName(ui->combox->currentText());
    serial->open(QIODevice::ReadWrite);

    //set  BaudRate
    serial->setBaudRate(115200);
    serial->setFlowControl(QSerialPort::NoFlowControl);

    //
    ui->openqsort->setStyleSheet("background:green");
   //连接信号槽
    QObject::connect(serial, &QSerialPort::readyRead, this, &MainWindow::read_data);

}

void MainWindow::on_closeqsort_clicked()
{
    if(serial->isOpen())
      {
         serial->clear();
         serial->close();
         serial->deleteLater();

         ui->combox->setEnabled(true);
//         ui->combox->setEditable(true);

         ui->openqsort->setStyleSheet("background:");
      }
    return ;

}

void MainWindow::read_data()
{
    QByteArray buf;
    buf = serial->readAll();

    if((buf.length() == 22)||(beginFlag == '#'))
    {

       QString str(tr(buf));
        dataHandle(str);

        //displaylift(str);
    }
    buf.clear() ;
}

bool MainWindow::toBool(QString tmp)
{
        return (bool)(tmp.toInt());
}

void MainWindow::dataHandle(QString tmp)
{
     //tmp.resize(20);
     QString s0 = tmp.at(0);
     QString s1 = tmp.at(1);
     QString s2 = tmp.at(2);
     QString s3 = tmp.at(3);
     QString s4 = tmp.at(4);
     QString s5 = tmp.at(5);
     QString s6 = tmp.at(6);
     QString s7 = tmp.at(7);
     QString s8 = tmp.at(8);
     QString s9 = tmp.at(9);
     QString s10 = tmp.at(10);
     QString s11 = tmp.at(11);
     QString s12 = tmp.at(12);
     QString s13 = tmp.at(13);
     QString s14 = tmp.at(14);
     QString s15 = tmp.at(15);
     QString s16 = tmp.at(16);
     QString s17 = tmp.at(17);
     QString s18 = tmp.at(18);
     QString s19 = tmp.at(19);
     QString s20 = tmp.at(20);
     QString s21 = tmp.at(21);

     beginFlag = *(s0.toLatin1().data());
     endFlag = *(s21.toLatin1().data());

     lift = s20.toInt();

     runFlag = toBool(s1);
     stopFlag = toBool(s2);
     l1U = toBool(s3);
     l2U = toBool(s4);
     l2D = toBool(s5);
     l3U = toBool(s6);
     l3D = toBool(s7);
     l4D = toBool(s8);
     l1 = toBool(s9);
     l2 = toBool(s10);
     l3 = toBool(s11);
     l4 = toBool(s12);
     opendor = toBool(s13);
     readyopen = toBool(s14);
     closedor = toBool(s15);
     readyclose = toBool(s16);
     upFlag = toBool(s17);
     dowmFlag = toBool(s18);
     lightflag = toBool(s19);

     panduan(ui->oneup,l1U);
     panduan(ui->twoup,l2U);
     panduan(ui->twodowm,l2D);
     panduan(ui->threeup,l3U);
     panduan(ui->threedowm,l3D);
     panduan(ui->fourdowm,l4D);
     panduan(ui->dooropen,readyopen);
     panduan(ui->doorclose,readyclose);

     panduan(ui->one,l1);
     panduan(ui->two,l2);
     panduan(ui->three,l3);
     panduan(ui->four,l4);
     panduan(ui->opendoor,opendor);
     panduan(ui->closedoor,closedor);
     panduan(ui->stop,stopFlag);
     panduan(ui->run,runFlag);
     panduan(ui->light,lightflag);
     panduan(ui->up,upFlag);
     panduan(ui->dowm,dowmFlag);
     ui->liftNumber->display(lift);

}
