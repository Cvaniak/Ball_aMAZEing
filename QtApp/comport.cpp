#include "comport.h"
#include "ui_comport.h"
#include <QSerialPortInfo>
#include <QSerialPort>
#include <QList>
#include <QDebug>
#include <QDateTime>
#include <QString>
#include <QtMath>

ComPort::ComPort(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ComPort)
{
    ui->setupUi(this);
    this->device = new QSerialPort(this);


}

ComPort::~ComPort()
{
    delete ui;
}

void ComPort::on_bFind_clicked()
{
    this->addToLogs("Szukam urządzeń...");
    QList<QSerialPortInfo> devices;
    devices = QSerialPortInfo::availablePorts();

    for(int i = 0; i < devices.count(); i++) {
      this->addToLogs("Znalazłem urządzenie: " + devices.at(i).portName() + " " + devices.at(i).description());
      ui->cBox->addItem(devices.at(i).portName() + "\t" + devices.at(i).description());
    }
}

void ComPort::on_bConnect_clicked()
{
    if(ui->cBox->count() == 0) {
      this->addToLogs("Nie wykryto żadnych urządzeń!");
      return;
    }

    QString portName = ui->cBox->currentText().split("\t").first();
    this->device->setPortName(portName);

    if(!device->isOpen()) {
        // OTWÓRZ I SKONFIGURUJ PORT:
        if(device->open(QSerialPort::ReadWrite)) {
          this->device->setBaudRate(QSerialPort::Baud115200);
          this->device->setDataBits(QSerialPort::Data8);
          this->device->setParity(QSerialPort::NoParity);
          this->device->setStopBits(QSerialPort::OneStop);
          this->device->setFlowControl(QSerialPort::NoFlowControl);

          connect(this->device, SIGNAL(readyRead()), this, SLOT(readFromPort()));

          this->addToLogs("Otwarto port szeregowy.");
          emit isStmConnected(1);
        } else {
          this->addToLogs("Otwarcie porty szeregowego się nie powiodło!");
        }
    } else {
      this->addToLogs("Port już jest otwarty!");
      return;
    }

}

void ComPort::on_bDisconnect_clicked()
{
    if(this->device->isOpen()) {
      this->device->close();
      this->addToLogs("Zamknięto połączenie.");
      emit isStmConnected(0);
    } else {
      this->addToLogs("Port nie jest otwarty!");
      return;
    }
}

void ComPort::addToLogs(QString message){
    QString currentDateTime = QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss");
    ui->tEdit->append(currentDateTime + "\t" + message);
}

void ComPort::readFromPort() {
  while(this->device->canReadLine()) {
    QString line = this->device->readLine();

    emit dataStm(line);
//    qDebug() << "comport " <<  line;
    QString terminator = "\r";;
    int pos = line.lastIndexOf(terminator);
    this->addToLogs(line.left(pos));
//    this->addToLogs(list.at(1));

  }
}
