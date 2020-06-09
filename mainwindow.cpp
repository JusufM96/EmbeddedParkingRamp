#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QtWidgets>
#include <string>
#include <QMessageBox>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
        counter = 0;
    ui->setupUi(this);
    arduino_is_available = false;
    arduino_port_name = "";
    arduino = new QSerialPort;

    foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts()){
           if(serialPortInfo.hasVendorIdentifier() && serialPortInfo.hasProductIdentifier()){
               if(serialPortInfo.vendorIdentifier() == arduino_uno_vendor_id){
                   if(serialPortInfo.productIdentifier() == arduino_uno_product_id){
                       arduino_port_name = serialPortInfo.portName();
                       arduino_is_available = true;
                   }
               }
           }
       }

       if(arduino_is_available){
           // open and configure the serialport
           arduino->setPortName(arduino_port_name);

           arduino->setBaudRate(QSerialPort::Baud9600);
           arduino->setDataBits(QSerialPort::Data8);

           arduino->setParity(QSerialPort::NoParity);
           arduino->setStopBits(QSerialPort::OneStop);
           arduino->setFlowControl(QSerialPort::NoFlowControl);
           arduino->open(QSerialPort::ReadWrite);

             QObject::connect(arduino, SIGNAL(readyRead()), this, SLOT(readSerial()));
       }else{
           // give error message if not available
           QMessageBox::warning(this, "Port error", "Couldn't find the Arduino!");
       }
   }



MainWindow::~MainWindow()
{
    if(arduino->isOpen()){
            arduino->close();
        }
        delete ui;

}

void MainWindow::readSerial(){


    QStringList buffer_split = serialBuffer.split("\r\n");
            // no parsed value yet so continue accumulating bytes from serial in the buffer.
            serialData = arduino->readAll();
            serialBuffer = serialBuffer + QString::fromStdString(serialData.toStdString());
            serialData.clear();
                    qDebug() <<buffer_split;

}
void MainWindow::updateRamp(QString command)
{
    if(arduino->isWritable()){
        arduino->write(command.toStdString().c_str());
    }else{
        qDebug() << "Couldn't write to serial!";
    }}


void MainWindow::on_pushButton_clicked()
{
MainWindow::updateRamp(QString("o"));
}

void MainWindow::on_pushButton_2_clicked()
{
MainWindow::updateRamp(QString("c"));
}


void MainWindow::on_pushButton_5_clicked()
{
MainWindow::updateRamp(QString("+"));
}

void MainWindow::on_pushButton_4_clicked()
{
    MainWindow::updateRamp(QString("-"));
}

void MainWindow::on_pushButton_3_clicked()
{


 counter++;
  QString s = QString::number(counter);
  ui -> lineEdit_6->setText(s);
  MainWindow::updateRamp(QString("l"));
  MainWindow::updateRamp(QString("+"));

}



void MainWindow::on_pushButton_6_clicked()
{
    counter = 0;
    QString s = QString::number(counter);
    ui -> lineEdit_6->setText(s);
}
