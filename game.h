#ifndef GAME_H
#define GAME_H

#include <box2d.h>
#include <QtWidgets>
#include <QWidget>
#include <QtCore/QObject>
#include "mainwindow.h"
#include "mcontactlistener.h"

struct oType
{
    int type;
};
enum {
    BallObject,
    WallObject,
    StarObject,
} Objects;

struct Object
{
    int type;
    b2Body *body;
    b2Fixture *fixture;
};

class Game : public QWidget
{
    Q_OBJECT
public:

    explicit Game(QWidget *parent = nullptr);
    virtual ~Game(){};
    Object createBall(const b2Vec2& pos, float radius);
    Object createStar(const b2Vec2& pos, float radius);
    Object createWall(float x, float y, float w, float h, float angle=0);

    void paintEvent(QPaintEvent *) override;

    void drawWall(QPainter *p, const Object& o);
    void drawEllipse(QPainter *p, const Object& o);

    void start();
    void timerEvent(QTimerEvent *event) override;

private:
//    b2World *_world;
    std::shared_ptr<b2World> _world;
    int _timerId;
    QTransform _transform;
    QVector<Object> _objects;
    QWidget *view;
//    MContactListener contactListener;


protected:
     void keyPressEvent(QKeyEvent *event) override;
private slots:
//     void on_star_delete(Object o);

};

#endif // GAME_H
