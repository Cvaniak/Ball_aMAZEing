#include "game.h"
#include <math.h>
#include <QDebug>
#include "mcontactlistener.h"


Game::Game(QWidget *viewT, QWidget* parent):QWidget(parent), _timerId(0)
{
    b2Vec2 gravity(0.0f, 0.0f);
    view = viewT;
    points = 0;
    level = 0;
    isRunning = 0;
    isStmRunning = 0;
    isReset = 0;
    stmPos.roll = 0;
    stmPos.pitch = 0;
    gameScale = 1.f;
    gameWidth = parent->size().width()*gameScale;
    gameHeight = parent->size().height()*gameScale;
    gameStroke = 10.0f*gameScale;
//    qDebug() << parent->size().width();

//        _world = new b2World(gravity);
    _world = std::make_shared<b2World>(gravity);

    MContactListener *contactListener = new MContactListener(this);
    _world->SetContactListener(contactListener);

    QObject::connect(contactListener,SIGNAL(star_collect()),view,SLOT(on_star_collect()));
    QObject::connect(contactListener,SIGNAL(hole_collect()),view,SLOT(on_hole_collect()));
    QObject::connect(contactListener,SIGNAL(end_collect()),view,SLOT(on_end_collect()));
    QObject::connect(this,SIGNAL(mouseClicked()),view,SLOT(on_bStartStop_clicked()));

    resetGame();


}

void Game::resetGame(){

//    deleteAllObjects();
//    emit mouseClicked();
    points = 0;
    isRunning = 0;
    float gw = gameWidth;
    float gh = gameHeight;
    float gs = gameStroke;
    _transform.scale(1/gameScale, 1/gameScale);

    _objects.append(createWall(0.0f, 0.0f, gw, gs));
    _objects.append(createWall(0.0f, 0.0f, gs, gh));
    _objects.append(createWall(gw-gs, 0.0f, gs, gh));
    _objects.append(createWall(0.0f, gh-gs, gw, gs));

    _objects.append(createWall(gw*.0f, gh*.1f, gw*.8, gs*1));
    _objects.append(createWall(gw*.2f, gh*.3f, gw*.8, gs*1));
    _objects.append(createWall(gw*.0f, gh*.4f, gw*.3, gs*1));
    _objects.append(createWall(gw*.0f, gh*.5f, gw*.8, gs*1));
    _objects.append(createWall(gw*.0f, gh*.6f, gw*.3, gs*1));
    _objects.append(createWall(gw*.2f, gh*.7f, gw*.8, gs*1));
    _objects.append(createWall(gw*.0f, gh*.8f, gw*.8, gs*1));

    _objects.append(createWall(gw*.4f, gh*.2f, gs*1, gh*.2));
    _objects.append(createWall(gw*.6f, gh*.4f, gs*1, gh*.2));


    _objects.append(createStar(b2Vec2(gw*.05, gh*.45), 1.5*gs));
    _objects.append(createHole(b2Vec2(gw*.55, gh*.55), 1.5*gs));
    _objects.append(createHole(b2Vec2(gw*.05, gh*.35), 1.5*gs));
    _objects.append(createEnd(b2Vec2(gw*.55, gh*.25), 1.5*gs));
    _objects.append(createBall(b2Vec2(gw*0.35, gh*0.55), 1.5*gs));
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
    QRectF r(0, 0, gameWidth, gameHeight);
    if(isStmRunning){
        QRadialGradient radialGrad(QPointF(gameWidth/2+stmPos.roll/90.0f*gameWidth*2,
                                           gameHeight/2-stmPos.pitch/90.0f*gameHeight*2), 1000*gameScale);
        radialGrad.setColorAt(0, Qt::gray);
        radialGrad.setColorAt(1, Qt::black);
        p.fillRect(r, radialGrad);
    }
    else{

        QRadialGradient radialGrad(QPointF(gameWidth-mouse.x()*gameScale,
                                           gameHeight-mouse.y()*gameScale), 1000*gameScale);
        radialGrad.setColorAt(0, Qt::gray);
        radialGrad.setColorAt(1, Qt::black);
        p.fillRect(r, radialGrad);
    }


    foreach(const Object* o, _objects) {
        switch(o->type) {
        case BallObject:
            drawBall(&p, *o);
            break;
        case StarObject:
            drawStar(&p, *o);
            break;
        case WallObject:
            drawWall(&p, *o);
            break;
        case HoleObject:
            drawHole(&p, *o);
            break;
        case EndObject:
            drawEnd(&p, *o);
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
    p->setBrush(QColor(36, 36, 36));
    p->save();
    p->translate(r.center());
    p->rotate(angle*180/b2_pi);
    p->translate(-r.center());
    p->drawRect(r);
    p->restore();
}

void Game::drawBall(QPainter *p, const Object& o) {
    float x = o.body->GetPosition().x;
    float y = o.body->GetPosition().y;
    float r = o.fixture->GetShape()->m_radius;


    QPoint mouse = this->mapFromGlobal(QCursor::pos());

    if(isStmRunning){
//         QRadialGradient radialGrad(QPointF(18-stmPos.roll/90.0f*60.0f, 32-stmPos.pitch/90.0f*60.0f), 100);
         QRadialGradient radialGrad(QPointF(gameWidth/2+stmPos.roll/90.0f*gameWidth*2,
                                            gameHeight/2-stmPos.pitch/90.0f*gameHeight*2), 500*gameScale);

         radialGrad.setColorAt(0, QColor(179, 220, 242));
        radialGrad.setColorAt(1, QColor(12, 22, 56));
        p->setBrush(radialGrad);
    }
    else{
        QRadialGradient radialGrad(QPointF(gameWidth-mouse.x()*gameScale,
                                           gameHeight-mouse.y()*gameScale), 500*gameScale);
         radialGrad.setColorAt(0, QColor(179, 220, 242));
        radialGrad.setColorAt(1, QColor(12, 22, 56));
        p->setBrush(radialGrad);
    }

    QPen pen;
    pen.setWidth(0);
    p->setPen(pen);
    p->drawEllipse(QPointF(x, y), r, r);
}

void Game::drawHole(QPainter *p, const Object& o) {
    float x = o.body->GetPosition().x;
    float y = o.body->GetPosition().y;
    float r = o.fixture->GetShape()->m_radius;

    QPoint mouse = this->mapFromGlobal(QCursor::pos());

    if(isStmRunning){
        QRadialGradient radialGrad(QPointF(gameWidth/2+stmPos.roll/90.0f*gameWidth*2,
                                           gameHeight/2-stmPos.pitch/90.0f*gameHeight*2), 700*gameScale);
        radialGrad.setColorAt(0, QColor(11, 11, 11));
        radialGrad.setColorAt(0.5, QColor(100, 100, 100));
        radialGrad.setColorAt(1, QColor(11, 11, 11));
        p->setBrush(radialGrad);
    }
    else{
        QRadialGradient radialGrad(QPointF(gameWidth-mouse.x()*gameScale,
                                           gameHeight-mouse.y()*gameScale), 700*gameScale);
        radialGrad.setColorAt(0, QColor(11, 11, 11));
        radialGrad.setColorAt(0.5, QColor(100, 100, 100));
        radialGrad.setColorAt(1, QColor(11, 11, 11));
        p->setBrush(radialGrad);
    }
    QPen pen;
    pen.setWidth(0);
    p->setPen(pen);
    p->drawEllipse(QPointF(x, y), r, r);
}


void Game::drawStar(QPainter *p, const Object& o) {
    float x = o.body->GetPosition().x;
    float y = o.body->GetPosition().y;
    float r = o.fixture->GetShape()->m_radius;

    QPoint mouse = this->mapFromGlobal(QCursor::pos());

    if(isStmRunning){
        QRadialGradient radialGrad(QPointF(gameWidth/2+stmPos.roll/90.0f*gameWidth*2,
                                           gameHeight/2-stmPos.pitch/90.0f*gameHeight*2), 700*gameScale);
        radialGrad.setColorAt(0, QColor(11, 11, 11));
        radialGrad.setColorAt(0.5, QColor(155, 140, 65));
        radialGrad.setColorAt(1, QColor(11, 11, 11));
        p->setBrush(radialGrad);
    }
    else{
        QRadialGradient radialGrad(QPointF(gameWidth-mouse.x()*gameScale,
                                           gameHeight-mouse.y()*gameScale), 700*gameScale);
        radialGrad.setColorAt(0, QColor(11, 11, 11));
        radialGrad.setColorAt(0.5, QColor(155, 140, 65));
        radialGrad.setColorAt(1, QColor(11, 11, 11));
        p->setBrush(radialGrad);
    }
    QPen pen;
    pen.setWidth(0);
    p->setPen(pen);
    p->drawEllipse(QPointF(x, y), r, r);
}

void Game::drawEnd(QPainter *p, const Object& o) {
    float x = o.body->GetPosition().x;
    float y = o.body->GetPosition().y;
    float r = o.fixture->GetShape()->m_radius;

    QPoint mouse = this->mapFromGlobal(QCursor::pos());

    if(isStmRunning){
        QRadialGradient radialGrad(QPointF(gameWidth/2+stmPos.roll/90.0f*gameWidth*2,
                                           gameHeight/2-stmPos.pitch/90.0f*gameHeight*2), 700*gameScale);
        radialGrad.setColorAt(0, QColor(225, 11, 11));
        radialGrad.setColorAt(0.5, QColor(245, 100, 100));
        radialGrad.setColorAt(1, QColor(225, 11, 11));
        p->setBrush(radialGrad);
    }
    else{
        QRadialGradient radialGrad(QPointF(gameWidth-mouse.x()*gameScale,
                                           gameHeight-mouse.y()*gameScale), 700*gameScale);
        radialGrad.setColorAt(0, QColor(225, 11, 11));
        radialGrad.setColorAt(0.5, QColor(245, 100, 100));
        radialGrad.setColorAt(1, QColor(225, 11, 11));
        p->setBrush(radialGrad);
    }
    QPen pen;
    pen.setWidth(0);
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

void Game::deleteAllObjects(){
    if(isReset){
        QMutableVectorIterator<Object*> i(_objects);
        while (i.hasNext()) {
            if (i.next()->type != -1){
                _world->DestroyBody(i.value()->body);
                i.remove();
            }
            else{
                qDebug() << i.value()->type;
            }
        }
        resetGame();
        isReset = 0;
    }
}


void Game::timerEvent(QTimerEvent *event) {
    if(event->timerId() == _timerId) {
        if(isRunning){

            _world->Step(1.0f/60.0f, 8, 6);
            deleteObjects();
            deleteAllObjects();
            if(isStmRunning)
                controlStm();
            else
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
        stmPos.roll = Roll;
        stmPos.pitch = Pitch;

//        qDebug() << "RP" << roll << pitch;
    }
    else if(list.at(0) == "Y"){
        emit mouseClicked();
    }
}

void Game::control(){
    QPoint p = this->mapFromGlobal(QCursor::pos());
    _world->SetGravity(b2Vec2((p.x()*gameScale-gameWidth/2)/(gameWidth)*200*gameScale,
                              (p.y()*gameScale-gameHeight/2)/(gameHeight)*200*gameScale));
}

void Game::controlStm(){
        _world->SetGravity(b2Vec2(-stmPos.roll*10*gameScale,
                                  stmPos.pitch*10*gameScale));

}

void Game::on_is_stm(int isStm){
    qDebug() << isStm;
    isStmRunning = isStm;
}

