#ifndef MCONTACTLISTENER_H
#define MCONTACTLISTENER_H

#include <box2d.h>
#include <QWidget>
#include <QtWidgets>
#include <QtCore/QObject>
#include "game.h"

//struct MContact {
//    b2Fixture *fixtureA;
//    b2Fixture *fixtureB;
//    bool operator==(const MContact& other) const
//    {
//        return (fixtureA == other.fixtureA) && (fixtureB == other.fixtureB);
//    }
//};

class MContactListener : public QWidget, public b2ContactListener{
    Q_OBJECT
public:
//    std::vector<MContact> _contacts;

    explicit MContactListener(QWidget *parent = nullptr);
    virtual ~MContactListener(){};

    virtual void BeginContact(b2Contact* contact);
    virtual void EndContact(b2Contact* contact);
    virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
    virtual void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);
    void addStar(int starId);
    void setPlayer(int playerId);
    double x;
    int _player;
    QVector<int> _stars;
signals:
//    void star_collect(Object o);
};

#endif // MCONTACTLISTENER_H
