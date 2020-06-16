#ifndef MCONTACTLISTENER_H
#define MCONTACTLISTENER_H

#include <box2d.h>
#include <QWidget>
//#include <QtWidgets>
//#include <QtCore/QObject>
#include "game.h"

/**
 * @brief Class that handles colisons in Box2D
 */
class MContactListener : public QWidget, public b2ContactListener{
    Q_OBJECT
public:
//    std::vector<MContact> _contacts;

    /**
     * @brief MContactListener constructor
     * @param parent
     */
    explicit MContactListener(QWidget *parent = nullptr);
    /**
     * @brief MContactListener destructor
     * @param parent
     */
    virtual ~MContactListener(){};
      virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
    virtual void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);
    void addStar(int starId);
    void setPlayer(int playerId);
    double x;
    int _player;
    QVector<int> _stars;
signals:
    void star_collect();
    void hole_collect();
    void end_collect();
};

#endif // MCONTACTLISTENER_H
