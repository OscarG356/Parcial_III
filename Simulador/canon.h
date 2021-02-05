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
    Canon(float px, float py, int id_, float dist_);

    QRectF boundingRect() const;
    void paint (QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    float getPosx() const;
    float getPosy() const;
    int getId() const;
    float getDist() const;
    void setDist(float value);
    float getR() const;
};

#endif // CANON_H
