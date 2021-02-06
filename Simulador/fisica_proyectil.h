#ifndef FISICA_PROYECTIL_H
#define FISICA_PROYECTIL_H

#include <QObject>
#include <math.h>
#include <cmath>
#include <QDebug>

class Fisica_Proyectil: public QObject
{    
    float px;
    float py;
    float r;
    float vx;
    float vy;
    float angulo;
    float ax;
    float ay;
    float g;
    float v;
    float dt;

public:
    Fisica_Proyectil(float Px, float Py, float ang, float vel_i, float R);
    ~Fisica_Proyectil();
    void ActualizarPosicion();
    void CalcularVelocidad();

    float getPx() const;
    float getPy() const;
    float getAngulo() const;
    float getV() const;
};

#endif // FISICA_PROYECTIL_H
