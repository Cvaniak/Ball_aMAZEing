#include "mcontactlistener.h"
#include <QDebug>
#include <QtAlgorithms>

MContactListener::MContactListener(QWidget *parent):
    QWidget(parent){
}

void MContactListener::PreSolve(b2Contact* contact,
  const b2Manifold* oldManifold) {
    Object *a = (Object *)contact->GetFixtureA()->GetBody()->GetUserData();
    Object *b = (Object *)contact->GetFixtureB()->GetBody()->GetUserData();
    if(b->type==BallObject){
        if(a->type == StarObject){
            a->shouldDie = true;
            contact->SetEnabled(false);
            emit star_collect();
        }
        if(a->type == HoleObject){
            emit hole_collect();

        }
        if(a->type == EndObject){
            emit end_collect();

        }
    }
    else if(a->type==BallObject){
        if(b->type == StarObject){
            b->shouldDie = true;
            contact->SetEnabled(false);
            emit star_collect();
        }
        if(b->type == HoleObject){
            emit hole_collect();

        }
        if(b->type == EndObject){
            emit end_collect();

        }

    }
}

