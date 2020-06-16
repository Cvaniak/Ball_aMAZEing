#ifndef Bubble_H
#define Bubble_H

#include <box2d.h>
#include <QtWidgets>
#include <QWidget>
#include <QtCore/QObject>
#include "game.h"

/**
 * @brief The Bubble class that shows orintation of game plane. Based on game class and need refactor/rebuild
 */
class Bubble : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief Constructor of bubble widget
     * @param MainWindow pointer
     * @param Ui Widget pointer for bubble
     */
    explicit Bubble(QWidget *view, QWidget *parent = nullptr);
    virtual ~Bubble(){};

    void resetBubble();

    Object* createWall(float x, float y, float w, float h, float angle=0);
    Object* createBall(const b2Vec2& pos, float radius);
    void paintEvent(QPaintEvent *) override;

    void drawWall(QPainter *p, const Object& o);
    void drawBall(QPainter *p, const Object& o);

    void deleteObjects();
    void deleteAllObjects();
    void control();
    void controlStm();

    void start();
    void timerEvent(QTimerEvent *event) override;

    Object* ball;
    int points;
    int level;
    int isRunning;
    int isReset;
    int bX;
    int bY;
    int isStmRunning;
    float BubbleWidth;
    float BubbleHeight;
    float BubbleStroke;
    float BubbleScale;
    StmPos stmPos;

private:
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
     void setPoints(int points, int addSet);
};

#endif // Bubble_H
