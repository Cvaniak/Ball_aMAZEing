#ifndef MCONTACTLISTENER_H
#define MCONTACTLISTENER_H

#include <box2d.h>
#include <QWidget>
#include "game.h"

/**
 * @brief Class that handles colisons in Box2D
 */
class MContactListener : public QWidget, public b2ContactListener{
    Q_OBJECT
public:
    /**
     * @brief MContactListener constructor
     * @param parent
     */
    explicit MContactListener(QWidget *parent = nullptr);
    /**
     * @brief MContactListener destructor
     */
    virtual ~MContactListener(){};
    /**
     * @brief PreSolve disable conntact if collison with collectable obeject and emit signal
     * @param contact
     * @param oldManifold
     */
    virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);

signals:
    /**
     * @brief Signal after collison with star
     */
    void star_collect();
    /**
     * @brief Signal after collison with hole
     */
    void hole_collect();
    /**
     * @brief Signal after collison with end
     */
    void end_collect();
};

#endif // MCONTACTLISTENER_H
