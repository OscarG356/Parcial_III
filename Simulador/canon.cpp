#include "canon.h"

Canon::Canon(float px, float py, int id_)
{
    posx = px;
    posy = py;
    id = id_;
    setPos(posx,posy);
}

QRectF Canon::boundingRect() const
{
    return QRectF(-r,-r,2*r,2*r);
}

void Canon::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->drawEllipse(boundingRect());
    if(id==1)
        painter->setBrush(Qt::red);
    else
        painter->setBrush(Qt::blue);
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
