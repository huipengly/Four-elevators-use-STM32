#include "mythread.h"
#include <QDebug>

MyThread::MyThread()
{
    serial = new QSerialPort;
}

MyThread::~MyThread()
{
    delete serial;
}

void MyThread::setName(const QString &name)
{
    serial->setPortName(name);
}

void MyThread::setBuad(const QString &buad)
{
    serial->setBaudRate(buad);
}

void MyThread::setStopBits(const QString &num)
{
    serial->setStopBits(num);
}

void MyThread::setParity(const QString &num)
{
    serial->setParity(num);
}

void MyThread::setDataBits(const QString &num)
{
    serial->setDataBits(num);
}

void MyThread::closePort()
{

}

void MyThread::run()
{
    QSerialPort *my_serialport = new QSerialPort;

    while(!stopped)
    {
        if(stopped&&com_opened)
        {
            my_serialport->close();
            com_opened = false;
        }
        if(!com_opened)       //
        {
            com_opened = true;
            my_serialport -> setPortName(portnum);
            my_serialport -> open(QIODevice::ReadWrite);
            my_serialport -> setBaudRate(9600);
            my_serialport -> setDataBits(QSerialPort::Data8);
            my_serialport -> setParity(QSerialPort::NoParity);  //设置奇偶校验
            my_serialport -> setStopBits(QSerialPort::OneStop);
            my_serialport -> setFlowControl(QSerialPort::NoFlowControl);
        }
        if(this->com_opened&&this->tx_event)
        {
            this -> tx_event = false;
            my_serialport ->clear(QSerialPort::AllDirections);
            my_serialport -> write(this->TxData);

            if(my_serialport->waitForBytesWritten(5))
            {
                qDebug() << "Send Succeed!";
            }
            else
            {
                qDebug() << "Send TimeOut!";
            }
         }
         if(my_serialport->waitForReadyRead(5))
         {
             while(my_serialport->waitForReadyRead(5))
             {
                 this->sleep(20);
                 this->RxData = my_serialport->readAll();
                 emit(this->comReceive());
             }
         }
         if(stopped&&com_opened)
         {
             my_serialport->close();
             com_opened = false;
         }

    }


}

