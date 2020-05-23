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

private slots:

    void on_bConnect_clicked();
    void on_star_collect();
    void on_hole_collect();
    void on_end_collect();

private:
    Ui::MainWindow *ui;
    ComPort comport;
    Game *game;
//    Game *game;

};
#endif // MAINWINDOW_H
