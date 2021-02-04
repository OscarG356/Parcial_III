#ifndef PROYECTIL_GRAPH_H
#define PROYECTIL_GRAPH_H

#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QPainter>
#include <stdlib.h>
#include <time.h>
#include <QObject>
#include <QTimer>
#include <QDebug>
#include "fisica_proyectil.h"

class Proyectil_Graph: public QObject, public QGraphicsItem
{
    int id;
    float r;
    Fisica_Proyectil *Disparo;

public:
    Proyectil_Graph(float px, float py,float ang,float v);
    ~Proyectil_Graph();
    QRectF boundingRect() const;
    void paint(QPainter *painter,const QStyleOptionGraphicsItem *option, QWidget *widget);
    void actualizar(float v_limit);

    Fisica_Proyectil *getDisparo() const;
    int getId() const;
    void setId(int value);
};

#endif // PROYECTIL_GRAPH_H
