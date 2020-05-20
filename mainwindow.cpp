#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSerialPortInfo>
#include <QSerialPort>
#include <QList>
#include <QDebug>
#include <QDateTime>
#include <QString>
#include "comport.h"
#include "game.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    Game *game = new Game(this);

    game->resize(360, 640);
    game->start();
//    game1->setGeometry(50,50,300,400);
//    game->show();
//    ui->wGame = game;

//    game = game1;
}

MainWindow::~MainWindow()
{
//    delete game;
    delete ui;
}


void MainWindow::on_bConnect_clicked()
{
    comport.show();
}

//void MainWindow::on_star_collect(){
//    qDebug() << "YO";
//}
