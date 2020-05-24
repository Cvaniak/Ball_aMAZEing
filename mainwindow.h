#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include "game.h"
#include "comport.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void start_stop(int should);

private slots:

    void on_star_collect();
    void on_hole_collect();
    void on_end_collect();
    void on_data_stm(QString stm);
    void on_is_stm(int isStm);
    void on_set_points(int points, int addSet);

    void on_bStartStop_clicked();
    void stop_game();
//    void on_mouse_clicked();

private:
    Ui::MainWindow *ui;
    ComPort *comport;
    Game *game;
//    Game *game;

signals:
    void dataStm(QString stm);
    void isStmConnected(int isStm);

};
#endif // MAINWINDOW_H
