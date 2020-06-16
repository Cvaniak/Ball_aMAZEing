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

    plot = new QCustomPlot();
    plot->resize(600,400);

    plot->addGraph();
    plot->graph(0)->setScatterStyle(QCPScatterStyle::ssCircle);
    plot->graph(0)->setLineStyle(QCPGraph::lsNone);
    plot->graph(0)->setPen(QPen(QColor(40, 110, 255)));
    plot->graph(0)->setName("Pitch");

    plot->addGraph();
    plot->graph(1)->setScatterStyle(QCPScatterStyle::ssCircle);
    plot->graph(1)->setLineStyle(QCPGraph::lsNone);
    plot->graph(1)->setPen(QPen(QColor(255, 110, 40)));
    plot->graph(1)->setName("Roll");

    plot->legend->setVisible(true);
    plot->legend->setBrush(QColor(255, 255, 255, 150));
    plot->xAxis->setLabel("Time in seconds");
    plot->yAxis->setLabel("Degrees");
    qv_x.append(0);
    qv_pitch.append(0);
    qv_roll.append(0);
    plot->yAxis->setRange(-90, 90);



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


void MainWindow::on_bPlot_clicked()
{
//    delete plot;
//    plot->setAttribute( Qt::WA_DeleteOnClose );
    plot->show();
}

void MainWindow::on_dataToPlot()
{
    qv_x.append(qv_x.last()+1/60.0);
    qv_roll.append(game->stmPos.roll);
    qv_pitch.append(game->stmPos.pitch);
    plot->graph(0)->setData(qv_x, qv_pitch);
    plot->graph(1)->setData(qv_x, qv_roll);
    plot->xAxis->setRange(qv_x.last(), 8, Qt::AlignRight);
    plot->replot();
    plot->update();
}

void MainWindow::on_data_stm(QString stm){
//    qDebug() << stm;
    emit dataStm(stm);
}

void MainWindow::on_is_stm(int isStm){
//    qDebug() << isStm;
    stop_game();
    emit isStmConnected(isStm);
}
void MainWindow::on_star_collect(){
    qDebug() << "Collect " ;
    game->points++;
//    ui->tPoints->setNum();
    ui->lPoints->setNum(game->points);

}
void MainWindow::on_hole_collect(){
    qDebug() << "You fall down " ;
    start_stop(0);
    game->isReset= 1;
    game->points = 0;
    ui->lPoints->setNum(0);

}
void MainWindow::on_end_collect(){
    qDebug() << "You win " ;
    game->isReset= 1;
    game->level++;
    ui->lLevel->setNum(game->level);


}
