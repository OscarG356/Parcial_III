#include "fisica_proyectil.h"
#define pi 3.1416

float Fisica_Proyectil::getAngulo() const
{
    return angulo;
}

float Fisica_Proyectil::getV() const
{
    return v;
}

Fisica_Proyectil::Fisica_Proyectil(float Px, float Py, float ang, float vel_i, float R)
{
    px = Px;
    py = Py;
    angulo = (ang*pi)/180;
    v = vel_i;
    r = R;

    vx=0;
    vy=0;
    ax = 0;
    ay= 0;
    g = 9.8;
    dt = 0.1;
}

void Fisica_Proyectil::ActualizarPosicion()
{
    CalcularVelocidad();
    px=px+vx*dt;
    py=py+vy*dt-(0.5*g*dt+dt);
}

void Fisica_Proyectil::CalcularVelocidad()
{
    vx=v*cos(angulo);
    vy=v*sin(angulo)-g*dt;
    angulo=atan2(vy,vx);
    v=sqrt(pow(vy,2)+pow(vx,2));
}

Fisica_Proyectil::~Fisica_Proyectil()
{

}

float Fisica_Proyectil::getPx() const
{
    return px;
}

float Fisica_Proyectil::getPy() const
{
    return py;
}
