#ifndef MCONTACTLISTENER_H
#define MCONTACTLISTENER_H

#include <box2d.h>
#include <QWidget>

struct MContact {
    b2Fixture *fixtureA;
    b2Fixture *fixtureB;
    bool operator==(const MContact& other) const
    {
        return (fixtureA == other.fixtureA) && (fixtureB == other.fixtureB);
    }
};

class MContactListener :public QWidget ,  public b2ContactListener{
    Q_OBJECT
public:
    std::vector<MContact>_contacts;

    explicit MContactListener(QWidget *parent = nullptr);
    virtual ~MContactListener();

    virtual void BeginContact(b2Contact* contact);
    virtual void EndContact(b2Contact* contact);
    virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
    virtual void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);
    double x;
signals:
//    void star_collect();
};

#endif // MCONTACTLISTENER_H
