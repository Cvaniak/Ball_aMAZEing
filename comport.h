#ifndef COMPORT_H
#define COMPORT_H

#include <QWidget>
#include <QSerialPort>

namespace Ui {
class ComPort;
}

/**
 * @brief Used for communication with MicroController. Based on Forbot tutorial
 */
class ComPort : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Constructor of Comport
     * @param parent should be MainWindow
     */
    explicit ComPort(QWidget *parent = nullptr);
    ~ComPort();

private slots:
    /**
     * @brief Find button things - look for posibble MC
     */
    void on_bFind_clicked();
    /**
     * @brief Connect button things - connect to the selected MC
     */
    void on_bConnect_clicked();
    /**
     * @brief Disconnect button things
     */
    void on_bDisconnect_clicked();
    /**
     * @brief Recieving data from MC and emit dataStm()
     */
    void readFromPort();

private:
    /**
     * @brief ui
     */
    Ui::ComPort *ui;
    /**
     * @brief Pointer to device serial port
     */
    QSerialPort *device;

    /**
     * @brief Writes data to console in widget
     */
    void addToLogs(QString message);
signals:
    /**
     * @brief Sends data from MC
     * @param stm data send from MC
     */
    void dataStm(QString stm);
    /**
     * @brief Indicate if MC is connected
     * @param isStm is or not
     */
    void isStmConnected(int isStm);
};

#endif // COMPORT_H
