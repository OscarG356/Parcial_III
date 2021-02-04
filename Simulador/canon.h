#ifndef CANON_H
#define CANON_H

#include <QGraphicsItem>
#include <QPainter>
#include <QTimer>

class Canon: public QObject, public QGraphicsItem
{  
    float posx ,posy;
    int id;
    float dist;
    float r;

public:
    Canon(float px, float py, int id_);

    QRectF boundingRect() const;
    void paint (QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    float getPosx() const;
    float getPosy() const;
    int getId() const;
    float getDist() const;
    void setDist(float value);
};

#endif // CANON_H
