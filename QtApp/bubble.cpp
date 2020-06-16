#include "bubble.h"
#include <math.h>
#include <QDebug>
#include "mcontactlistener.h"


Bubble::Bubble(QWidget *viewT, QWidget* parent):QWidget(parent), _timerId(0)
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
    BubbleScale = 1.f;
    BubbleWidth = parent->size().width()*BubbleScale;
    BubbleHeight = parent->size().height()*BubbleScale;
    BubbleStroke = 10.0f*BubbleScale;
    bX = BubbleWidth/2;
    bY = BubbleHeight/2;
//    qDebug() << parent->size().width();

//        _world = new b2World(gravity);
    _world = std::make_shared<b2World>(gravity);

    MContactListener *contactListener = new MContactListener(this);
    _world->SetContactListener(contactListener);

//    QObject::connect(contactListener,SIGNAL(star_collect()),view,SLOT(on_star_collect()));
//    QObject::connect(contactListener,SIGNAL(hole_collect()),view,SLOT(on_hole_collect()));
//    QObject::connect(contactListener,SIGNAL(end_collect()),view,SLOT(on_end_collect()));
//    QObject::connect(this,SIGNAL(mouseClicked()),view,SLOT(on_bStartStop_clicked()));

    resetBubble();


}

void Bubble::resetBubble(){

//    deleteAllObjects();
//    emit mouseClicked();
    points = 0;
    isRunning = 0;
    float gw = BubbleWidth;
    float gh = BubbleHeight;
    float gs = BubbleStroke;
    _transform.scale(1/BubbleScale, 1/BubbleScale);

    ball =createBall(b2Vec2(gw*0.35, gh*0.55), 1.5*gs);
}

Object* Bubble::createWall(float x, float y, float w, float h, float angle) {
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

Object* Bubble::createBall(const b2Vec2& pos, float radius) {
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


void Bubble::paintEvent(QPaintEvent *) {
    QPainter p(this);

//        p.begin(this);
//        p.fillRect(rect(), Qt::RadialGradientPattern);
//        p.end();

    p.setRenderHint(QPainter::Antialiasing, true);
    p.setTransform(_transform);

    QRectF r(0, 0, BubbleWidth, BubbleHeight);

    QRadialGradient radialGrad(QPointF(BubbleWidth/2, BubbleHeight/2), 100);
    radialGrad.setColorAt(0, Qt::green);
    radialGrad.setColorAt(1, Qt::black);
    p.setBrush(radialGrad);
    p.drawEllipse(QPointF(BubbleWidth/2, BubbleHeight/2), 90, 90);


    drawBall(&p, *ball);
//    foreach(const Object* o, _objects) {
//        switch(o->type) {
//        case BallObject:
//            drawBall(&p, *o);
//            break;

//        }

//    }
}

void Bubble::drawWall(QPainter *p, const Object& o) {
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

void Bubble::drawBall(QPainter *p, const Object& o) {
    float x = bX;
    float y = bY;
    float r = 20;


    p->setBrush(QColor(195,210,160));

    QPen pen;
    pen.setWidth(2);
    p->setPen(pen);
    p->drawEllipse(QPointF(x, y), r, r);
}

void Bubble::start() {
    if(!_timerId) {
        _timerId = startTimer(1000/60); // 60fps
    }
}

void Bubble::deleteObjects(){
    QMutableVectorIterator<Object*> i(_objects);
    while (i.hasNext()) {
        if (i.next()->shouldDie == true){
            _world->DestroyBody(i.value()->body);
            i.remove();
        }
    }

}

void Bubble::deleteAllObjects(){
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
        resetBubble();
        isReset = 0;
    }
}


void Bubble::timerEvent(QTimerEvent *event) {
    if(event->timerId() == _timerId) {
        if(isRunning){

            _world->Step(1.0f/60.0f, 8, 6);
        }
        // Remove all numbers < 0 from QVector<int>
        if(isStmRunning)
            controlStm();
        else
            control();

        update();
    }
    else{
//            qDebug() << event->timerId() << endl;
    }
}

void Bubble::keyPressEvent(QKeyEvent *event)
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

void Bubble::mousePressEvent(QMouseEvent *event){
//    qDebug() << "yo";
    emit mouseClicked();
}

void Bubble::on_data_stm(QString line){

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

void Bubble::control(){
    QPoint p = this->mapFromGlobal(QCursor::pos());
//    ball->body->
    bX = ((p.x()+700.0)/700.0)*BubbleWidth + BubbleWidth/2;
    bY = ((p.y()+400.0)/700.0)*BubbleHeight+ BubbleHeight/2;
}

void Bubble::controlStm(){
        bX = BubbleWidth/2+stmPos.roll;
        bY = BubbleHeight/2-stmPos.pitch;

}

void Bubble::on_is_stm(int isStm){
    qDebug() << isStm;
    isStmRunning = isStm;
}

