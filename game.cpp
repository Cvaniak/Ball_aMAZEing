#include "game.h"
#include <math.h>
#include <QDebug>
#include "mcontactlistener.h"


Game::Game(QWidget *viewT, QWidget* parent):QWidget(parent), _timerId(0)
{
    b2Vec2 gravity(0.0f, -10.0f);
    view = viewT;
    points = 0;
    isRunning = 1;
    isStmRunning = 0;
//        _world = new b2World(gravity);
    _world = std::make_shared<b2World>(gravity);

    MContactListener *contactListener = new MContactListener(this);
    _world->SetContactListener(contactListener);

    QObject::connect(contactListener,SIGNAL(star_collect()),view,SLOT(on_star_collect()));
    QObject::connect(contactListener,SIGNAL(hole_collect()),view,SLOT(on_hole_collect()));
    QObject::connect(contactListener,SIGNAL(end_collect()),view,SLOT(on_end_collect()));
    QObject::connect(this,SIGNAL(mouseClicked()),view,SLOT(on_bStartStop_clicked()));


    for(int i=0; i<1; i++) {
        int dx = 18 - qrand() % 36;
        int dy = qrand() % 2;
        _objects.append(createBall(b2Vec2(18.0f+dx, 62.0f-dy), 1.0f));
    }

    _transform.scale(10.0f, -10.0f);
    _transform.translate(0.0f, -64.0f);
//        qDebug() << _transform.map(QPointF(0.0f,0.0f));
//        qDebug() << _transform.map(QPointF(36.0f,64.0f));

    _objects.append(createWall(0.0f, 0.0f, 36.0f, 1.0f));
    _objects.append(createWall(0.0f, 0.0f, 1.0f, 64.0f));
    _objects.append(createWall(35.0f, 0.0f, 1.0f, 64.0f));
    _objects.append(createWall(0.0f, 63.0f, 36.0f, 1.0f));

    _objects.append(createWall(0.0f, 32.0f, 18.0f, 1.0f));
    _objects.append(createWall(18.0f, 16.0f, 18.0f, 1.0f));

    _objects.append(createStar(b2Vec2(28.0f, 52.0f), 1.0f));
    _objects.append(createHole(b2Vec2(14.0f, 52.0f), 1.0f));
    _objects.append(createEnd(b2Vec2(14.0f, 40.0f), 1.0f));
//    _objects.append(createWall(18.0f, 50.0f, 18.0f, 1.0f));


//        _objects.append(createWall(4.0f, 48.0f, 8.0f, 1.0f));
//        _objects.append(createWall(24.0f, 48.0f, 8.0f, 1.0f, -0.25f*b2_pi));

}

Object* Game::createWall(float x, float y, float w, float h, float angle) {
    Object *o = new Object;
    // body
//        const b2PolygonShape *shape = dynamic_cast<b2PolygonShape*>(o.fixture->GetShape());
    o->shouldDie = false;
    b2BodyDef bd;
    bd.type = b2_staticBody;
    bd.position = b2Vec2(x+w/2.0f, y+h/2.0f);
    bd.angle = angle * b2_pi;
//        b2Body *o = _world->CreateBody(&bd);
    o->body = _world->CreateBody(&bd);
//    oType *t = new oType;
//    t->type = 0;
    // shape
    b2PolygonShape shape;
//        const b2PolygonShape *shape = dynamic_cast<b2PolygonShape*>();

    shape.SetAsBox(w/2.0f, h/2.0f);
    // fixture
    b2FixtureDef fd;
    fd.shape = &shape;
    fd.density = 0.1f;
    fd.friction = 0.3f;
    o->fixture = o->body->CreateFixture(&fd);
//        o.m_type = WallObject;
    o->type = WallObject;
    o->body->SetUserData((void*)o);
//                .type = WallObj/ect;
    return o;
}

Object* Game::createBall(const b2Vec2& pos, float radius) {
    Object *o = new Object;
    o->shouldDie = false;
    // body
    b2BodyDef bd;
    bd.type = b2_dynamicBody;
    bd.position = pos;
    bd.allowSleep = false;

    o->body = _world->CreateBody(&bd);
    // shape
    b2CircleShape shape;
    shape.m_radius = radius;
    // fixture
    b2FixtureDef fd;
    fd.shape = &shape;
    fd.density = 1.0f;
    fd.friction = 1.0f;
    fd.restitution = 0.6f;
    o->fixture = o->body->CreateFixture(&fd);
    o->type = BallObject;
    o->body->SetUserData((void*)o);
    return o;
}

Object* Game::createStar(const b2Vec2& pos, float radius) {
    Object *o = new Object;
    o->shouldDie = false;
    // body
    b2BodyDef bd;
    bd.type = b2_staticBody;
    bd.position = pos;
    bd.allowSleep = true;
    o->body = _world->CreateBody(&bd);
    // shape
    b2CircleShape shape;
    shape.m_radius = radius;
    // fixture
    b2FixtureDef fd;
    fd.shape = &shape;
    fd.density = 1.0f;
    fd.friction = 1.0f;
    fd.restitution = 0.6f;
    o->fixture = o->body->CreateFixture(&fd);
    o->type = StarObject;
    o->body->SetUserData((void*)o);

    return o;
}

Object* Game::createHole(const b2Vec2& pos, float radius) {
    Object *o = new Object;
    o->shouldDie = false;
    // body
    b2BodyDef bd;
    bd.type = b2_staticBody;
    bd.position = pos;
    bd.allowSleep = true;
    o->body = _world->CreateBody(&bd);
    // shape
    b2CircleShape shape;
    shape.m_radius = radius;
    // fixture
    b2FixtureDef fd;
    fd.shape = &shape;
    fd.density = 1.0f;
    fd.friction = 1.0f;
    fd.restitution = 0.6f;
    o->fixture = o->body->CreateFixture(&fd);
    o->type = HoleObject;
    o->body->SetUserData((void*)o);

    return o;
}

Object* Game::createEnd(const b2Vec2& pos, float radius) {
    Object *o = new Object;
    o->shouldDie = false;
    // body
    b2BodyDef bd;
    bd.type = b2_staticBody;
    bd.position = pos;
    bd.allowSleep = true;
    o->body = _world->CreateBody(&bd);
    // shape
    b2CircleShape shape;
    shape.m_radius = radius;
    // fixture
    b2FixtureDef fd;
    fd.shape = &shape;
    fd.density = 1.0f;
    fd.friction = 1.0f;
    fd.restitution = 0.6f;
    o->fixture = o->body->CreateFixture(&fd);
    o->type = EndObject;
    o->body->SetUserData((void*)o);

    return o;
}

void Game::paintEvent(QPaintEvent *) {
    QPainter p(this);

//        p.begin(this);
//        p.fillRect(rect(), Qt::RadialGradientPattern);
//        p.end();

    p.setRenderHint(QPainter::Antialiasing, true);
    p.setTransform(_transform);


    QPoint mouse = this->mapFromGlobal(QCursor::pos());

//    double mouseTx = qMax(0.0,18-qFabs(mouse.x()/10-18));

//    double mouseTy = qMax(0.0, 32-qFabs(mouse.y()/10-32));
//    qDebug() << mouseTy << " " << 32-qFabs(mouse.y()/10-32);

//    double mouseTx1;
//    if(mouse.x()/10==18){
//        mouseTx1 = 0;
//    }
//    else
//        mouseTx1 = (18-mouse.x()/10)/qFabs(18-mouse.x()/10)*1*mouseTx;

//    double mouseTy1;
//    if(mouse.y()/10==32){
//        mouseTy1 = 00;
//    }
//    else
//        mouseTy1 =(32-mouse.y()/10)/qFabs(32-mouse.y()/10)*1*mouseTy;

//    qDebug() << mouseTy1 ;

//        QLinearGradient linearGrad(
//                    QPointF(18+mouseTx1, 32-mouseTy1),
//                    QPointF(18-mouseTx1,32+mouseTy1));
//        linearGrad.setColorAt(0, Qt::red);
//        linearGrad.setColorAt(1, Qt::black);
//
//        p.fillRect(r, linearGrad);
    QRectF r(0, 0, 36, 64);
    QRadialGradient radialGrad(QPointF(36-mouse.x()/10, mouse.y()/10), 100);

    radialGrad.setColorAt(0, Qt::gray);
    radialGrad.setColorAt(1, Qt::black);
    p.fillRect(r, radialGrad);

//   else{

//        QRectF r(0, 0, 36, 64);
//        p.fillRect(r, QColor(0,0,0));
//    }




    foreach(const Object* o, _objects) {
        switch(o->type) {
        case BallObject:
            drawEllipse(&p, *o);
            break;
        case StarObject:
            drawEllipse(&p, *o);
            break;
        case WallObject:
            drawWall(&p, *o);
            break;
        case HoleObject:
            drawHole(&p, *o);
            break;
        case EndObject:
            drawHole(&p, *o);
            break;

        }

    }
}

void Game::drawWall(QPainter *p, const Object& o) {
    float x = o.body->GetPosition().x;
    float y = o.body->GetPosition().y;
    float angle = o.body->GetAngle();
    const b2PolygonShape *shape = dynamic_cast<b2PolygonShape*>(o.fixture->GetShape());
    float hx = shape->m_vertices->x;
    float hy = shape->m_vertices->y;
    QRectF r(x-hx, y-hy, 2*hx, 2*hy);
    p->setPen(QColor(36, 36, 36));
    p->save();
    p->translate(r.center());
    p->rotate(angle*180/b2_pi);
    p->translate(-r.center());
    p->drawRect(r);
    p->restore();
}

void Game::drawEllipse(QPainter *p, const Object& o) {
    float x = o.body->GetPosition().x;
    float y = o.body->GetPosition().y;
    float r = o.fixture->GetShape()->m_radius;


    QPoint mouse = this->mapFromGlobal(QCursor::pos());
    QRadialGradient radialGrad(QPointF(36-mouse.x()/10, mouse.y()/10), 75);
    radialGrad.setColorAt(0, QColor(179, 220, 242));
    radialGrad.setColorAt(1, QColor(12, 22, 56));
    p->setBrush(radialGrad);


    QPen pen;
    pen.setWidth(0.2);
    p->setPen(pen);
    p->drawEllipse(QPointF(x, y), r, r);
}

void Game::drawHole(QPainter *p, const Object& o) {
    float x = o.body->GetPosition().x;
    float y = o.body->GetPosition().y;
    float r = o.fixture->GetShape()->m_radius;


    QPoint mouse = this->mapFromGlobal(QCursor::pos());
    QRadialGradient radialGrad(QPointF(36-mouse.x()/10, mouse.y()/10), 75);
    radialGrad.setColorAt(0, QColor(179, 220, 242));
    radialGrad.setColorAt(1, QColor(12, 22, 56));
    p->setBrush(QColor(255, 240, 105));


    QPen pen;
    pen.setWidth(0.2);
    p->setPen(pen);
    p->drawEllipse(QPointF(x, y), r, r);
}

void Game::start() {
    if(!_timerId) {
        _timerId = startTimer(1000/60); // 60fps
    }
}

void Game::deleteObjects(){
    QMutableVectorIterator<Object*> i(_objects);
    while (i.hasNext()) {
        if (i.next()->shouldDie == true){
            _world->DestroyBody(i.value()->body);
            i.remove();
        }
    }

}

void Game::control(){
    QPoint p = this->mapFromGlobal(QCursor::pos());
    _world->SetGravity(b2Vec2(qMin((p.x()-180)/18, 10), -qMin((p.y()-320)/32, 10)));

}

void Game::timerEvent(QTimerEvent *event) {
    if(event->timerId() == _timerId) {
        if(isRunning){

            _world->Step(1.0f/60.0f, 8, 6);
            deleteObjects();
            if(!isStmRunning)
                control();

        }
        // Remove all numbers < 0 from QVector<int>
        update();
    }
    else{
//            qDebug() << event->timerId() << endl;
    }
}

void Game::keyPressEvent(QKeyEvent *event)
{
    b2Vec2 gravity(0.0f, 0.0f);
    if(event->key() == Qt::Key_Left)
    {
        gravity += b2Vec2(-10.0f, 0.0f);
    }
    if(event->key() == Qt::Key_Right)
    {
        gravity += b2Vec2(10.0f, 0.0f);
    }
    if(event->key() == Qt::Key_Down)
    {
        gravity += b2Vec2(0.0f, -10.0f);
    }
    if(event->key() == Qt::Key_Up)
    {
        gravity += b2Vec2(0.0f, 10.0f);
    }
    _world->SetGravity(gravity);
}

void Game::mousePressEvent(QMouseEvent *event){
//    qDebug() << "yo";
    emit mouseClicked();
}

void Game::on_data_stm(QString line){

    QRegExp rx("[ ]");// match a comma or a space
    QStringList list = line.split(rx, QString::SkipEmptyParts);

    if(list.at(0) == "X"){
        qint16 x = list.at(1).toInt();
        qint16 y = list.at(2).toInt();
        qint16 z = list.at(3).toInt();
//        qDebug() << x << y << z;
        float Roll = atan2(y, z) * 180/M_PI;
        float Pitch = atan2(-x, sqrt(y*y + z*z)) * 180/M_PI;
        float roll =  (Roll);
        float pitch = (Pitch);
        controlStm(roll, pitch);

//        qDebug() << "RP" << roll << pitch;
    }
    else if(list.at(0) == "Y"){
        emit mouseClicked();
    }
}

void Game::controlStm(float roll, float pitch){
    if(isStmRunning){
        _world->SetGravity(b2Vec2(roll, pitch));

    }
}

void Game::on_is_stm(int isStm){
    qDebug() << isStm;
    isStmRunning = isStm;
}
