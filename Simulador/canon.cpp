#include "canon.h"
#include <QDebug>

float Canon::getR() const
{
    return r;
}

Canon::Canon(float px, float py, int id_, float dist_)
{
    posx = px;
    posy = py;
    id = id_;
    dist = dist_;
    if(id==1 || id ==2){
        r = 7;
    }
    else if(id == 3){
        r = 0.05*dist;
    }
    else{
        r = 0.025*dist;
    }
    setPos(posx,posy);
}

QRectF Canon::boundingRect() const
{
    return QRectF(-r,-r,2*r,2*r);
}

void Canon::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if(id==3 || id==4)
        painter->drawEllipse(boundingRect());
    else if(id==1){
        painter->setBrush(Qt::red);
        painter->drawEllipse(boundingRect());
    }
    else if(id==2){
        painter->setBrush(Qt::blue);
        painter->drawEllipse(boundingRect());
    }
}

float Canon::getPosy() const
{
    return posy;
}

int Canon::getId() const
{
    return id;
}

float Canon::getDist() const
{
    return dist;
}

void Canon::setDist(float value)
{
    dist = value;
}

float Canon::getPosx() const
{
    return posx;
}
