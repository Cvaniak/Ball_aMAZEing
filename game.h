#ifndef GAME_H
#define GAME_H

#include <box2d.h>
#include <QtWidgets>
#include <QWidget>
#include <QtCore/QObject>
//#include "mainwindow.h"
//#include "mcontactlistener.h"


enum {
    BallObject,
    WallObject,
    StarObject,
    HoleObject,
    EndObject,
};

/**
 * @brief Struct for handling diffrent kind of object in game
 */
struct Object
{
    /**
     * @brief Type of object
     */
    int type;
    /**
     * @brief Box2D object
     */
    b2Body *body;
    /**
     * @brief Fixture from Box2D
     */
    b2Fixture *fixture;
    /**
     * @brief Should Die indicate for game loop to handle destrtuction
     */
    bool shouldDie;

};
/**
 * @brief Struct for pitch and roll of MC orientation
 */
struct StmPos
{
    /**
     * @brief pitch value in degres
     */
    int pitch;
    /**
     * @brief roll value in degres
     */
    int roll;
};

/**
 * @brief QPainter display and all game mechanic
 */
class Game : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief Constructor for game window
     * @param view MainWindow pointer
     * @param parent Ui Widget pointer for game window
     */
    explicit Game(QWidget *view, QWidget *parent = nullptr);
    virtual ~Game(){};

    /**
     * @brief Reset game (delete all object and create new one)
     */
    void resetGame();

    /**
     * @brief Create wall
     * @param x poistion in pixels
     * @param y poistion in pixels
     * @param w width in pixels
     * @param h height in pixels
     * @param angle in degres
     * @return Custom data struct with Box2D and QPainter object
     */
    Object* createWall(float x, float y, float w, float h, float angle=0);
    /**
     * @brief Create Ball object
     * @param pos Box2D vector with x and y position in pixels
     * @param radius size of circle (radius) in pixels
     * @return
     */
    Object* createBall(const b2Vec2& pos, float radius);
    /**
     * @brief Create Star object
     * @param pos Box2D vector with x and y position in pixels
     * @param radius size of circle (radius) in pixels
     * @return
     */
    Object* createStar(const b2Vec2& pos, float radius);
    /**
     * @brief Create Hole object
     * @param pos Box2D vector with x and y position in pixels
     * @param radius size of circle (radius) in pixels
     * @return
     */
    Object* createHole(const b2Vec2& pos, float radius);
    /**
     * @brief Create End object
     * @param pos Box2D vector with x and y position in pixels
     * @param radius size of circle (radius) in pixels
     * @return
     */
    Object* createEnd(const b2Vec2& pos, float radius);

    /**
     * @brief Display method that draw all object and background light
     */
    void paintEvent(QPaintEvent *) override;

    /**
     * @brief Defines how to draw Wall
     * @param p QPainter canvas pointer
     * @param o Object to draw
     */
    void drawWall(QPainter *p, const Object& o);
    /**
     * @brief Defines how to draw Ball
     * @param p QPainter canvas pointer
     * @param o Object to draw
     */
    void drawBall(QPainter *p, const Object& o);
    /**
     * @brief Defines how to draw Hole
     * @param p QPainter canvas pointer
     * @param o Object to draw
     */
    void drawHole(QPainter *p, const Object& o);
    /**
     * @brief Defines how to draw Star
     * @param p QPainter canvas pointer
     * @param o Object to draw
     */
    void drawStar(QPainter *p, const Object& o);
    /**
     * @brief Defines how to draw End
     * @param p QPainter canvas pointer
     * @param o Object to draw
     */
    void drawEnd(QPainter *p, const Object& o);

    /**
     * @brief Remove box2d objects that are sign do delete
     */
    void deleteObjects();
    /**
     * @brief Remove all box2d objects
     */
    void deleteAllObjects();
    /**
     * @brief Set gravity in Box2D World based on mouse input
     */
    void control();
    /**
     * @brief Set gravity in Box2D World based MC input
     */
    void controlStm();

    /**
     * @brief Starts main timer that handle updates of world
     */
    void start();

    /**
     * @brief Updates, deletes, control world, display
     */
    void timerEvent(QTimerEvent *event) override;
    /**
     * @brief Current number of points
     */
    int points;
    /**
     * @brief Current number of level
     */
    int level;
    /**
     * @brief Is game running or paused
     */
    int isRunning;
    /**
     * @brief Is game set to reset
     */
    int isReset;
    /**
     * @brief Is MC connected
     */
    int isStmRunning;
    /**
     * @brief Game window width
     */
    float gameWidth;
    /**
     * @brief Game window height
     */
    float gameHeight;
    /**
     * @brief Default stroke for QPainter
     */
    float gameStroke;
    /**
     * @brief Game scale (to change zoom to map)
     */
    float gameScale;
    /**
     * @brief Orientation of MC
     */
    StmPos stmPos;

private:
    /**
     * @brief Pointer to Box2D world
     */
    std::shared_ptr<b2World> _world;
    /**
     * @brief Main loop timer ID
     */
    int _timerId;
    /**
     * @brief QPainter world transform
     */
    QTransform _transform;
    /**
     * @brief List of all objects in game
     */
    QVector<Object*> _objects;
    /**
     * @brief Pointer to parent
     */
    QWidget *view;

//    MContactListener contactListener;

protected:
    /**
      * @brief Just for debug. Changes gravity to sites using arrows
      * @param event
      */
     void keyPressEvent(QKeyEvent *event) override;
     /**
      * @brief Get info about mouse pressed
      * @param event
      */
     void mousePressEvent(QMouseEvent *event) override;

private slots:
     /**
      * @brief Recieve emited data from MC
      * @param line of input data
      */
     void on_data_stm(QString line);
     /**
      * @brief Setup control for mouse or MC
      * @param isStm Is MC connected or not
      */
     void on_is_stm(int isStm);

signals:
     /**
      * @brief Mouse Clicked event
      */
     void mouseClicked();
     /**
      * @brief Emit info about points event
      * @param points Number of points
      * @param addSet Add or set mode
      */
     void setPoints(int points, int addSet);
};

#endif // GAME_H
