#ifndef GAME_H
#define GAME_H

#include <box2d.h>
#include <QtWidgets>
#include <QWidget>
#include <QtCore/QObject>
//#include "mainwindow.h"
//#include "mcontactlistener.h"

struct oType
{
    int type;
};
enum {
    BallObject,
    WallObject,
    StarObject,
    HoleObject,
    EndObject,
} Objects;

struct Object
{
    int type;
    b2Body *body;
    b2Fixture *fixture;
    bool shouldDie;

};

struct StmPos
{
    int pitch;
    int roll;
};

class Game : public QWidget
{
    Q_OBJECT
public:

    explicit Game(QWidget *view, QWidget *parent = nullptr);
    virtual ~Game(){};
    Object* createBall(const b2Vec2& pos, float radius);
    Object* createStar(const b2Vec2& pos, float radius);
    Object* createWall(float x, float y, float w, float h, float angle=0);
    Object* createHole(const b2Vec2& pos, float radius);
    Object* createEnd(const b2Vec2& pos, float radius);

    void paintEvent(QPaintEvent *) override;

    void drawWall(QPainter *p, const Object& o);
    void drawEllipse(QPainter *p, const Object& o);
    void drawHole(QPainter *p, const Object& o);

    void deleteObjects();
    void control();
    void controlStm();

    void start();
    void timerEvent(QTimerEvent *event) override;
    int points;
    int isRunning;
    int isStmRunning;
    StmPos stmPos;

private:
//    b2World *_world;
    std::shared_ptr<b2World> _world;
    int _timerId;
    QTransform _transform;
    QVector<Object*> _objects;
    QWidget *view;

//    MContactListener contactListener;

protected:
     void keyPressEvent(QKeyEvent *event) override;
     void mousePressEvent(QMouseEvent *event) override;

private slots:
     void on_data_stm(QString line);
     void on_is_stm(int isStm);

signals:
     void mouseClicked();
//     void on_star_collect();
//     void on_hole_collect();
//     void on_end_collect();
};

#endif // GAME_H
