#include "mcontactlistener.h"
#include <QDebug>
#include <QtAlgorithms>

MContactListener::MContactListener(QWidget *parent):
    QWidget(parent){
}

//MContactListener::~MContactListener() {
//}

void MContactListener::BeginContact(b2Contact* contact) {
    // We need to copy out the data because the b2Contact passed in
    // is reused.
//    MContact myContact = { contact->GetFixtureA(), contact->GetFixtureB() };
//    _contacts.push_back(myContact);
}

void MContactListener::EndContact(b2Contact* contact) {
//    MContact myContact = { contact->GetFixtureA(), contact->GetFixtureB() };
//    std::vector<MContact>::iterator pos;
//    pos = std::find(_contacts.begin(), _contacts.end(), myContact);
//    if (pos != _contacts.end()) {
//        _contacts.erase(pos);
//    }
}

void MContactListener::PreSolve(b2Contact* contact,
  const b2Manifold* oldManifold) {
    x++;
//    emit star_collect();
//    contact->ge
    Object *a = (Object *)contact->GetFixtureA()->GetBody()->GetUserData();
    Object *b = (Object *)contact->GetFixtureB()->GetBody()->GetUserData();
    qDebug() << a->type << " " << b->type;
    if(a->type == StarObject && b->type==BallObject){
//        b->body->GetWorld()->DestroyBody(b->body);
//        emit star_collect(b);
    }
    else if(b->type == StarObject && a->type==BallObject){
//        a->body->GetWorld()->DestroyBody(a->body);
//          emit star_collect(a);

    }
//    if(qCount(_stars.begin(), _stars.end(), contact->GetFixtureA()))

    //    contact->SetEnabled(false);
}

void MContactListener::PostSolve(b2Contact* contact,
  const b2ContactImpulse* impulse) {
}
