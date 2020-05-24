#ifndef COMPORT_H
#define COMPORT_H

#include <QWidget>
#include <QSerialPort>

namespace Ui {
class ComPort;
}

class ComPort : public QWidget
{
    Q_OBJECT

public:
    explicit ComPort(QWidget *parent = nullptr);
    ~ComPort();

private slots:
    void on_bFind_clicked();
    void on_bConnect_clicked();
    void on_bDisconnect_clicked();
    void readFromPort();

private:
    Ui::ComPort *ui;
    QSerialPort *device;

    void addToLogs(QString message);
signals:
    void dataStm(QString stm);
    void isStmConnected(int isStm);
};

#endif // COMPORT_H
