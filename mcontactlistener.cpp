#import "mcontactlistener.h"

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
    emit star_collect();
//    contact->SetEnabled(false);
}

void MContactListener::PostSolve(b2Contact* contact,
  const b2ContactImpulse* impulse) {
}
