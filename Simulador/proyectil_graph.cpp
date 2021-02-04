#include "proyectil_graph.h"

Proyectil_Graph::Proyectil_Graph(float px, float py, float ang, float v)
{
    float posx,posy;
    posx = px;
    posy = py;
    setPos(posx,posy);
    r = 5;

    Disparo = new Fisica_Proyectil(posx,posy,ang,v,r);
}

Proyectil_Graph::~Proyectil_Graph()
{

}

QRectF Proyectil_Graph::boundingRect() const
{
    return QRectF(-r,-r,2*r, 2*r);
}

void Proyectil_Graph::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->drawEllipse(boundingRect());
    if(id==1)
        painter->setBrush(Qt::red);
    else
        painter->setBrush(Qt::blue);
}

void Proyectil_Graph::actualizar(float v_limit)
{
    Disparo->ActualizarPosicion();
    setPos(Disparo->getPx(),v_limit-Disparo->getPy());
}

int Proyectil_Graph::getId() const
{
    return id;
}

void Proyectil_Graph::setId(int value)
{
    id = value;
}

Fisica_Proyectil *Proyectil_Graph::getDisparo() const
{
    return Disparo;
}