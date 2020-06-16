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
    game = new Game(this, ui->wGame);

    game->resize(ui->wGame->size());
//    game->resize(1000, 1000);
//    game->setParent();
    game->start();

    bubble = new Bubble(this, ui->wBubble);

    bubble->resize(ui->wBubble->size());
//    game->resize(1000, 1000);
//    game->setParent();
    bubble->start();
    comport = new ComPort(ui->wConnect);
//    comport->setParent(ui->wConnect);
//    ui->wConnect = comport;

    QObject::connect(comport,SIGNAL(dataStm(QString)),this,SLOT(on_data_stm(QString)));
    QObject::connect(this,SIGNAL(dataStm(QString)),game,SLOT(on_data_stm(QString)));
    QObject::connect(this,SIGNAL(dataStm(QString)),bubble,SLOT(on_data_stm(QString)));

    QObject::connect(comport,SIGNAL(isStmConnected(int)),this,SLOT(on_is_stm(int)));
    QObject::connect(this,SIGNAL(isStmConnected(int)),game,SLOT(on_is_stm(int)));
    QObject::connect(this,SIGNAL(isStmConnected(int)),bubble,SLOT(on_is_stm(int)));

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


void MainWindow::start_stop(int should = 2){
    if(should == 0){
        game->isRunning = 0;
        ui->bStartStop->setText("Resume");
    }
    else if(should == 1){
        game->isRunning = 1;
        ui->bStartStop->setText("Pause");
    }
    else{
        if(game->isRunning == 1){
            game->isRunning = 0;
            ui->bStartStop->setText("Resume");
        }
        else{
            game->isRunning = 1;
            ui->bStartStop->setText("Pause");
        }
    }
}

void MainWindow::on_bStartStop_clicked()
{
   start_stop();
}

void MainWindow::stop_game()
{
   start_stop(0);
}

