#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include "game.h"
#include "bubble.h"
#include "comport.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

/**
 * @brief App MainWidow
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief MainWindow constructor
     * @param parent
     */
    MainWindow(QWidget *parent = nullptr);
    /**
     * @brief MainWindow destructor
     * @param parent
     */
    ~MainWindow();
    /**
     * @brief Switch between start and stop
     * @param Mode 0=Stop 1=Run 2=Toggle
     */
    void start_stop(int should);

private slots:

    /**
     * @brief Handle behaviour after collecting star
     */
    void on_star_collect();
    /**
     * @brief Handle behaviour after collecting hole
     */
    void on_hole_collect();
    /**
     * @brief Handle behaviour after collecting end
     */
    void on_end_collect();
    /**
     * @brief redistribute MC data
     */
    void on_data_stm(QString stm);
    /**
     * @brief Stops game after MC connection
     * @param isStm
     */
    void on_is_stm(int isStm);
    /**
     * @brief Set new number of points
     * @param number of points
     * @param Add or sets points
     */
    void on_set_points(int points, int addSet);

    /**
     * @brief Handle start stop button behaviour
     */
    void on_bStartStop_clicked();
    /**
     * @brief Stops game
     */
    void stop_game();
//    void on_mouse_clicked();

private:
    Ui::MainWindow *ui;
    /**
     * @brief Comport widget pointer
     */
    ComPort *comport;
    /**
     * @brief Game window widget pointer
     */
    Game *game;
    /**
     * @brief Bubble widget pointer
     */
    Bubble *bubble;
//    Game *game;

signals:
    /**
     * @brief Recieve emited data from MC
     * @param Line of input data
     */
    void dataStm(QString stm);
    /**
     * @brief Setup control for mouse or MC
     * @param Is MC connected or not
     */
    void isStmConnected(int isStm);

};
#endif // MAINWINDOW_H
