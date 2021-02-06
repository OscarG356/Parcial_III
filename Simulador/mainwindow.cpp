#include "mainwindow.h"
#include "ui_mainwindow.h"
#define pi 3.1416
#define G 9.8

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    h_limit = 601;
    v_limit = 461;

    scene->setSceneRect(0,0,h_limit,v_limit);
    ui->graphicsView->setScene(scene);
    ui->centralwidget->adjustSize();
    scene->addRect(scene->sceneRect());
    this->resize(800,600);
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    ui->pushButton_6->hide();
    ui->splitter_7->hide();
    ui->pushButton_7->hide();
    ui->splitter_8->hide();

    move = new QTimer(this);
    connect(move,SIGNAL(timeout()),this,SLOT(Actualizar()));

    trayectoria = new QTimer(this);
    connect(trayectoria,SIGNAL(timeout()),this,SLOT(Marca()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Preparar_Simulacion()
{
    //Se crean los cañones con los parametros solicitados por el usuario y se añaden a escena

    Canones.push_back(new Canon(10,v_limit-ui->PYO->value()-5,1,ui->PXD->value()));
    scene->addItem(Canones.back());
    Canones.push_back(new Canon(ui->PXD->value(),v_limit-ui->PYD->value(),2,ui->PXD->value()));
    scene->addItem(Canones.back());
    Canones.push_back(new Canon(10,v_limit-ui->PYO->value()-5,4,ui->PXD->value()));
    scene->addItem(Canones.back());
    Canones.push_back(new Canon(ui->PXD->value(),v_limit-ui->PYD->value(),3,ui->PXD->value()));
    scene->addItem(Canones.back());
}

bool MainWindow::Impacto(float XO, float YO, float XD, float YD, float Rang)
{
    //Verifica colisiones entre dos cuerpos
    bool flag= false;

    if(sqrt(pow((XO - XD),2)+pow((YO - YD),2)) < Rang){
        //        qDebug()<<sqrt(pow((XO - XD),2)+pow((YO - YD),2));
        flag = true;
    }

    return flag;
}

void MainWindow::Simulacion_1(float XD, float YD, float R)
{
    //Se crean y se inician las variables necesarias para la simulación
    int flag = 0;
    float xf,yf;
    float Vxo,Vy0;
    int V0o = 0;
    int t = 0;
    int angulo = 0;
    for(V0o = 0; V0o < 500 ; V0o+=5){ //Se itera en las velocidades posibles
        for(angulo = 0; angulo < 90; angulo++){ //Se busca el angulo de disparo adecuado
            Vxo = V0o*cos(angulo*pi/180); //Se calculan las velocidades
            Vy0 = V0o*sin(angulo*pi/180);
            xf = 0.0;
            yf = 0.0;
            for(t = 0; ; t++){ //Se hace un recorrido en el tiempo que la bala estará viajando
                xf = 10+Vxo*t; //Se le da la posicion inicial del cañon
                yf = (v_limit-Canones.at(0)->getPosy()) + Vy0*t -(0.5*G*t*t);
                if(Impacto(xf,yf,XD,YD,R)){ //Se verifica si el disparo es efectivo
                    //Se crean los disparos, sabiendo que estos ya son efectivos
                    Disparos.push_back(new Proyectil_Graph(10,ui->PYO->value(),angulo,V0o,XD,1));
                    scene->addItem(Disparos.back());
                    Disparos.push_back(new Proyectil_Graph(10,ui->PYO->value(),angulo,V0o,XD,3));
                    scene->addItem(Disparos.back());
                    // Se levanta una bandera que indica el numero de disparos solicitados
                    flag += 1;
                    // Se aumenta la velocidad en 50 para mayor variedad en los datos
                    V0o += 50;
                    break;
                }
                //*** Se verifica si es imposible el disparo ***
                if(yf < 0 || xf>XD+R){
                    break;
                }
                //************ Se rompen los ciclos en caso de que se encuentren los disparos necesarios o no se puedan más **************
            }
            if(flag == 3) break;

        }
        if(flag == 3) break;
    }
    if(flag < 3){
        msgBox.setText("No se encontraron al menos 3 disparos efectivos");
        msgBox.exec();
    }
    //************************************************************************************************************************
}

void MainWindow::Simulacion_2(float YO, float XD, float YD, float R)
{
    // Se crean las varibles necesarias para la simulación
    int flag = 0;
    float Vxo,Vy0,xf,yf;
    int V0o = 0;
    int t = 0;
    int angulo = 0;
    //*** Se itera igual que en la simulación anterior ***
    for(V0o = 0;V0o < 500; V0o+=5){
        for(angulo = 0; angulo < 90; angulo++){
            //*** Se le suman 90 al angulo para que el disparo sea hacia atrás ***
            Vxo = V0o*cos((angulo+90)*pi/180);
            Vy0 = V0o*sin((angulo+90)*pi/180);
            xf = 0.0;
            yf = 0.0;
            for(t = 0; ; t++){
                xf = XD+Vxo*t;
                yf = (v_limit-Canones.at(1)->getPosy()) + Vy0*t -(0.5*G*t*t);
                if(Impacto(xf,yf,10,YO,R)){
                    // El disparo se crea con el ángulo mas 90 para que se dispare hacia el cañoñ ofensivo
                    Disparos.push_back(new Proyectil_Graph(XD,YD,angulo+90,V0o,XD,2));
                    scene->addItem(Disparos.back());
                    Disparos.push_back(new Proyectil_Graph(XD,YD,angulo+90,V0o,XD,4));
                    scene->addItem(Disparos.back());
                    flag += 1;
                    V0o += 50;
                    break;
                }
                if(yf < 0 || xf>XD+R){
                    break;
                }
            }
            if(flag == 3) break;

        }
        if(flag == 3) break;
    }
    if(flag < 3){
        msgBox.setText("No se encontraron al menos 3 disparos efectivos");
        msgBox.exec();
    }
}

void MainWindow::Simulacion_3(float Ang, float VO0)
{
    // Se reciben los datos del disparo ofensivo
    // Se crean las variables de simulación
    int flag = 0;
    bool flag2;
    float xf,yf, x2,y2;
    float aux,auy;
    float Vxo,Vy0, Vxoo,Vyoo;
    int V0o = 0;
    int Time = 2;
    float angulo = 0;
    // Se calcula la velocidad en X y Y para el disparo ofensivo
    Vxoo = VO0*cos((Ang)*pi/180);
    Vyoo = VO0*sin((Ang)*pi/180);
    // Se itera para encontrar el disparo igual que en los anteriores
    for(V0o = 0;V0o<500; V0o +=5){
        for(angulo = 0; angulo < 90; angulo++){
            Vxo = V0o*cos((angulo+90)*pi/180);
            Vy0 = V0o*sin((angulo+90)*pi/180);
            xf = 0.0;
            yf = 0.0;
            x2 = 0.0;
            y2 = 0.0;
            for(int t=0; ; t++){
                // Se calculan las posiciones de ambos disparos, teniendo en cuenta el retraso del informante
                xf = Canones.at(1)->getPosx()+Vxo*t;
                yf = (v_limit-Canones.at(1)->getPosy()) + Vy0*t -(0.5*G*t*t);
                x2 = Canones.at(0)->getPosx()+Vxoo*(t+Time);
                y2 = (v_limit-Canones.at(0)->getPosy()) + Vyoo*(t+Time) -(0.5*G*(t+Time)*(t+Time));
                // Se verifica si los disparos impactan para un instante de tiempo diferente al evaluado
                // Lo cual seria imposible
                for(int t2 = t; ;t2++){
                    aux = Canones.at(1)->getPosx()+Vxo*t2;
                    auy = Canones.at(1)->getPosy() + Vy0*t2 -(0.5*G*t2*t2);
                    if(Impacto(x2,y2,aux,auy,Canones.at(2)->getR())){
                        flag2 = 1;
                        // Se levanta una bandera bool ya que se determino que el disparo es imposible
                        break;
                    }
                    if(auy < 0){
                        break;
                    }
                }
                if(flag2){
                    flag2 = 0;
                    break;
                }
                // En caso de que el disparo sea posible y efectivo se crean los disparos
                if(Impacto(xf,yf,x2,y2,Canones.at(2)->getR())){
                    Disparos.push_back(new Proyectil_Graph(Canones.at(1)->getPosx(),v_limit-Canones.at(1)->getPosy(),angulo+90,V0o,Canones.at(1)->getPosx(),2));
                    scene->addItem(Disparos.back());
                    Disparos.push_back(new Proyectil_Graph(Canones.at(1)->getPosx(),v_limit-Canones.at(1)->getPosy(),angulo+90,V0o,Canones.at(1)->getPosx(),4));
                    scene->addItem(Disparos.back());
                    flag += 1;
                    V0o += 50;
                    break;
                }
                if(yf < 0){
                    break;
                }
            }
            if(flag == 3) break;

        }
        if(flag == 3) break;
    }
    if(flag < 3){
        msgBox.setText("No se encontraron al menos 3 disparos efectivos");
        msgBox.exec();
    }
}

void MainWindow::Simulacion_4(float Ang, float VO0)
{
    // Funciona casi igual que el metodo anterior, pero con una verificacion en la linea 275
    int flag = 0;
    bool flag2;
    float xf,yf, x2,y2;
    float aux,auy;
    float Vxo,Vy0, Vxoo,Vyoo;
    int V0o = 0;
    int Time = 2;
    float angulo = 0;
    Vxoo = VO0*cos((Ang)*pi/180);
    Vyoo = VO0*sin((Ang)*pi/180);
    for(V0o = 0;V0o<500; V0o +=5){
        for(angulo = 0; angulo < 90; angulo++){
            Vxo = V0o*cos((angulo+90)*pi/180);
            Vy0 = V0o*sin((angulo+90)*pi/180);
            xf = 0.0;
            yf = 0.0;
            x2 = 0.0;
            y2 = 0.0;
            for(int t=0; ; t++){
                xf = Canones.at(1)->getPosx()+Vxo*t;
                yf = (v_limit-Canones.at(1)->getPosy()) + Vy0*t -(0.5*G*t*t);
                x2 = Canones.at(0)->getPosx()+Vxoo*(t+Time);
                y2 = (v_limit-Canones.at(0)->getPosy()) + Vyoo*(t+Time) -(0.5*G*(t+Time)*(t+Time));
                for(int t2 = t; ;t2++){
                    aux = Canones.at(1)->getPosx()+Vxo*t2;
                    auy = Canones.at(1)->getPosy() + Vy0*t2 -(0.5*G*t2*t2);
                    if(Impacto(x2,y2,aux,auy,Canones.at(2)->getR())){
                        flag2 = 1;
                        break;
                    }
                    if(auy < 0){
                        break;
                    }
                }
                if(flag2){
                    flag2 = 0;
                    break;
                }
                // Se verifica que el disparo defensivo no afecte al ofensivo
                if(Impacto(xf,yf,Canones.at(0)->getPosx(),Canones.at(0)->getPosy(),Canones.at(2)->getR())){
                    // En caso de dañar el defensivo se descarta el disparo y se busca otro
                    break;
                }

                if(Impacto(xf,yf,x2,y2,Canones.at(2)->getR())){
                    Disparos.push_back(new Proyectil_Graph(Canones.at(1)->getPosx(),v_limit-Canones.at(1)->getPosy(),angulo+90,V0o,Canones.at(1)->getPosx(),2));
                    scene->addItem(Disparos.back());
                    Disparos.push_back(new Proyectil_Graph(Canones.at(1)->getPosx(),v_limit-Canones.at(1)->getPosy(),angulo+90,V0o,Canones.at(1)->getPosx(),4));
                    scene->addItem(Disparos.back());
                    flag += 1;
                    V0o += 50;
                    break;
                }
                if(yf < 0){
                    break;
                }
            }
            if(flag == 3) break;

        }
        if(flag == 3) break;
    }
    if(flag < 3){
        msgBox.setText("No se encontraron al menos 3 disparos efectivos");
        msgBox.exec();
    }
}

void MainWindow::Simulacion_5(float Ang_o, float VO0,float Ang_d, float VD0)
{
    // Recibe los parametros de los disparos ofensivos y defensivos
    // Se crean las variables del disparo ofensivo defensivo

    int flag = 0;
    bool flag2;
    float xf,yf, x2,y2, x3,y3;
    float aux,auy;
    float Vxo,Vy0, Vxoo,Vyoo,Vxd,Vyd;
    int V0o = 0;
    int Time = 2, time3=3, t2;
    float angulo = 0;
    // Se calculan las velocidades de ambos disparos
    Vxoo = VO0*cos((Ang_o)*pi/180);
    Vyoo = VO0*sin((Ang_o)*pi/180);
    Vxd= VD0*cos((Ang_d+90)*pi/180);
    Vyd= VD0*sin((Ang_d+90)*pi/180);
    // Se busca el disparo igual que en el punto anterior, teniendo en cuenta los retrasos y las balas en escena
    for(V0o = 0;V0o<500; V0o +=5){
        for(angulo = 0; angulo < 90; angulo++){
            Vxo = V0o*cos((angulo)*pi/180);
            Vy0 = V0o*sin((angulo)*pi/180);
            xf = 0.0;
            yf = 0.0;
            x3 = 0.0;
            y3 = 0.0;
            x2 = 0.0;
            y2 = 0.0;
            for(int t=0; ; t++){
                // Se calculan las posiciones teniendo en cuenta los retrasos del informante
                x3 = Canones.at(1)->getPosx()+Vxd*t;
                y3 = (v_limit-Canones.at(1)->getPosy()) + Vyd*t -(0.5*G*t*t);
                x2 = Canones.at(0)->getPosx()+Vxoo*(t+Time);
                y2 = (v_limit-Canones.at(0)->getPosy()) + Vyoo*(t+Time) -(0.5*G*(t+Time)*(t+Time));
                xf = Canones.at(0)->getPosx()+Vxo*(t+time3);
                yf = (v_limit-Canones.at(0)->getPosy()) + Vy0*(t+time3) -(0.5*G*(t+time3)*(t+time3));

                // Se tiene en cuenta de que el disparo ofensivo-defensivo no afecte al disparo ofensivo efectivo
                if(!Impacto(x3,y3,x2,y2,Canones.at(2)->getR())){
                    // Se verifica que los disparos sean posibles
                    for(t2 = t; ;t2++){
                        aux = Canones.at(0)->getPosx()+Vxo*t2;
                        auy = Canones.at(0)->getPosy() + Vy0*t2 -(0.5*G*t2*t2);
                        if(Impacto(x3,y3,aux,auy,(Canones.at(1)->getPosx()*0.005))){
                            flag2 = 1;
                            break;
                        }
                        if(auy < 0){
                            break;
                        }
                    }
                    if(flag2){
                        flag2 = 0;
                        break;
                    }
                    // Se verifica que el disparo ofensivo-defensivo impacte con el disparo defensivo efectivo
                    if (Impacto(xf,yf,x3,y3,(Canones.at(1)->getPosx()*0.005))){
                        Disparos.push_back(new Proyectil_Graph(Canones.at(0)->getPosx(),v_limit-Canones.at(0)->getPosy(),angulo,V0o,Canones.at(1)->getPosx(),1));
                        scene->addItem(Disparos.back());
                        Disparos.push_back(new Proyectil_Graph(Canones.at(0)->getPosx(),v_limit-Canones.at(0)->getPosy(),angulo,V0o,Canones.at(1)->getPosx(),5));
                        scene->addItem(Disparos.back());
                        flag += 1;
                        V0o += 50;
                        break;
                    }



                }
                if(yf < 0){
                    break;
                }
            }
            if(flag == 3) break;

        }
        if(flag == 3) break;
    }
    if(flag < 3){
        msgBox.setText("No se encontraron al menos 3 disparos efectivos");
        msgBox.exec();
    }
}

//************************************************************************************************
/* Botones que informan sobre la simulación a realizar, muestran los DoubleSpinBox en caso de
ser necesario ingresar datos, muestran el boton de disparo y cambian la semilla de simulación */

void MainWindow::on_pushButton_clicked()
{
    msgBox.setText("Generar disparos ofensivos que comprometan la integridad del cañón defensivo.");
    msgBox.exec();
    Preparar_Simulacion();
    ui->pushButton_6->show();
    Seed=1;
}

void MainWindow::on_pushButton_2_clicked()
{
    msgBox.setText("Generar disparos defensivos que comprometan la integridad del cañón ofensivo.");
    msgBox.exec();
    Preparar_Simulacion();
    ui->pushButton_6->show();
    Seed=2;
}

void MainWindow::on_pushButton_3_clicked()
{
    msgBox.setText("Dado un disparo ofensivo, generar disparos defensivos que impida que el cañón defensivo"
                   "sea destruido sin importar si el cañón ofensivo pueda ser destruido.");
    msgBox.exec();
    Preparar_Simulacion();
    ui->pushButton_6->show();
    Seed=3;
    ui->splitter->hide();
    ui->splitter_7->show();
}

void MainWindow::on_pushButton_4_clicked()
{
    msgBox.setText("Dado un disparo ofensivo, generar disparo defensivos que impidan que los cañones defensivo"
                   "y ofensivo puedan ser destruidos.");
    msgBox.exec();
    Preparar_Simulacion();
    ui->pushButton_6->show();
    Seed=4;
    ui->splitter->hide();
    ui->splitter_7->show();
}

void MainWindow::on_pushButton_5_clicked()
{
    msgBox.setText("Dado un disparo ofensivo efectivo y un disparo defensivo que comprometa la efectividad del"
                   "ataque ofensivo, generar disparos que neutralicen el ataque defensivo y permitan que el"
                   " ataque ofensivo sea efectivo.");
    msgBox.exec();
    Preparar_Simulacion();
    ui->pushButton_6->show();
    Seed=5;
    ui->splitter->hide();
    ui->splitter_8->show();
    ui->splitter_7->show();
}

//************************************************************************************************

//********************** Botón de disparo, llama a cada método de simulación según la semilla dada *********
void MainWindow::on_pushButton_6_clicked()
{
    ui->pushButton_6->hide();
    ui->splitter->show();
    ui->splitter_7->hide();
    ui->splitter_8->hide();
    ui->pushButton_7->show();

    if(Seed==1){
        Simulacion_1(Canones.at(1)->getPosx(),v_limit-Canones.at(1)->getPosy(),Canones.at(0)->getR());
    }
    else if(Seed==2){
        Simulacion_2(v_limit-Canones.at(0)->getPosy(),Canones.at(1)->getPosx(),v_limit-Canones.at(1)->getPosy(),Canones.at(2)->getR());
    }
    else if(Seed==3){
        Simulacion_3(ui->doubleSpinBox_2->value(),ui->doubleSpinBox->value());
        Disparos.push_back(new Proyectil_Graph(Canones.at(0)->getPosx(),v_limit-Canones.at(0)->getPosy(),ui->doubleSpinBox_2->value(),ui->doubleSpinBox->value(),Canones.at(1)->getPosx(),1));
        scene->addItem(Disparos.back());
        Disparos.push_back(new Proyectil_Graph(Canones.at(0)->getPosx(),v_limit-Canones.at(0)->getPosy(),ui->doubleSpinBox_2->value(),ui->doubleSpinBox->value(),Canones.at(1)->getPosx(),3));
        scene->addItem(Disparos.back());
    }
    else if(Seed==4){
        Simulacion_4(ui->doubleSpinBox_2->value(),ui->doubleSpinBox->value());
        Disparos.push_back(new Proyectil_Graph(Canones.at(0)->getPosx(),v_limit-Canones.at(0)->getPosy(),ui->doubleSpinBox_2->value(),ui->doubleSpinBox->value(),Canones.at(1)->getPosx(),1));
        scene->addItem(Disparos.back());
        Disparos.push_back(new Proyectil_Graph(Canones.at(0)->getPosx(),v_limit-Canones.at(0)->getPosy(),ui->doubleSpinBox_2->value(),ui->doubleSpinBox->value(),Canones.at(1)->getPosx(),3));
        scene->addItem(Disparos.back());
    }
    else if(Seed==5){
        Simulacion_5(ui->doubleSpinBox_2->value(),ui->doubleSpinBox->value(),ui->doubleSpinBox_4->value(),ui->doubleSpinBox_3->value());
        Disparos.push_back(new Proyectil_Graph(Canones.at(0)->getPosx(),v_limit-Canones.at(0)->getPosy(),ui->doubleSpinBox_2->value(),ui->doubleSpinBox->value(),Canones.at(1)->getPosx(),1));
        scene->addItem(Disparos.back());
        Disparos.push_back(new Proyectil_Graph(Canones.at(0)->getPosx(),v_limit-Canones.at(0)->getPosy(),ui->doubleSpinBox_2->value(),ui->doubleSpinBox->value(),Canones.at(1)->getPosx(),3));
        scene->addItem(Disparos.back());
        Disparos.push_back(new Proyectil_Graph(Canones.at(1)->getPosx(),v_limit-Canones.at(1)->getPosy(),ui->doubleSpinBox_4->value()+90,ui->doubleSpinBox_3->value(),Canones.at(1)->getPosx(),2));
        scene->addItem(Disparos.back());
        Disparos.push_back(new Proyectil_Graph(Canones.at(1)->getPosx(),v_limit-Canones.at(1)->getPosy(),ui->doubleSpinBox_4->value()+90,ui->doubleSpinBox_3->value(),Canones.at(1)->getPosx(),4));
        scene->addItem(Disparos.back());
    }
    move->start(100);
    trayectoria->start(500);
}

//************************** Limpia la escena **********************
void MainWindow::on_pushButton_7_clicked()
{
    move->stop();
    ui->pushButton_7->hide();
    scene->clear();
    Canones.clear();
    Disparos.clear();
    Rastro.clear();
}
//******************************************************************

//******  Método encargado de mover la balas por la escena ******
void MainWindow::Actualizar()
{
    for(int i=0;i<Disparos.size();i++){
        Disparos.at(i)->actualizar(v_limit);
    }
}

//****************************************** Método encargado de dejar la trayectoría de los disparos en pantalla *************************************************************************************************************************************************
void MainWindow::Marca()
{
    for(int i=0;i<Disparos.size();i++){
        Rastro.push_back(new Proyectil_Graph(Disparos.at(i)->getDisparo()->getPx(),v_limit-Disparos.at(i)->getDisparo()->getPy(),Disparos.at(i)->getDisparo()->getAngulo(),Disparos.at(i)->getDisparo()->getV(),Canones.at(1)->getPosx(),Disparos.at(i)->getId()));
        scene->addItem(Rastro.back());
    }
}
