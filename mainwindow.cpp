#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSerialPortInfo>
#include <QSerialPort>
#include <QList>
#include <QDebug>
#include <QDateTime>
#include <QString>
#include "comport.h"
//#include "game.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    game = new Game(this);

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


void MainWindow::on_star_collect(){
    qDebug() << "Collect " ;
    game->points++;
//    ui->tPoints->setNum();
    ui->lPoints->setNum(game->points);

}
void MainWindow::on_hole_collect(){
    qDebug() << "You fall down " ;

}
void MainWindow::on_end_collect(){
    qDebug() << "You win " ;

}
