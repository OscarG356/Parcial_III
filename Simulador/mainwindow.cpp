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

    move = new QTimer(this);
    connect(move,SIGNAL(timeout()),this,SLOT(Actualizar()));

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
    bool flag= false;

    if(sqrt(pow((XO - XD),2)+pow((YO - YD),2)) < Rang){
//        qDebug()<<sqrt(pow((XO - XD),2)+pow((YO - YD),2));
        flag = true;
    }

    return flag;
}

void MainWindow::Simulacion_1(float XD, float YD, float R)
{

    int flag = 0;
    float xf,yf;
    float Vxo,Vy0;
    int V0o = 0;
    int t = 0;
    int angulo = 0;
    for(V0o = 0; V0o < 500 ; V0o+=5){
        for(angulo = 0; angulo < 90; angulo++){
            Vxo = V0o*cos(angulo*pi/180);
            Vy0 = V0o*sin(angulo*pi/180);
            xf = 0.0;
            yf = 0.0;
            for(t = 0; ; t++){
                xf = 10+Vxo*t;
                yf = (v_limit-Canones.at(0)->getPosy()) + Vy0*t -(0.5*G*t*t);
                if(Impacto(xf,yf,XD,YD,R)){
                    Disparos.push_back(new Proyectil_Graph(10,ui->PYO->value(),angulo,V0o,XD,1));
                    scene->addItem(Disparos.back());
                    Disparos.push_back(new Proyectil_Graph(10,ui->PYO->value(),angulo,V0o,XD,3));
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

void MainWindow::Simulacion_2(float YO, float XD, float YD, float R)
{
    int flag = 0;
    float Vxo,Vy0,xf,yf;
    int V0o = 0;
    int t = 0;
    int angulo = 0;
    for(V0o = 0;V0o < 500; V0o+=5){
        for(angulo = 0; angulo < 90; angulo++){
            Vxo = V0o*cos((angulo+90)*pi/180);
            Vy0 = V0o*sin((angulo+90)*pi/180);
            xf = 0.0;
            yf = 0.0;
            for(t = 0; ; t++){
                xf = XD+Vxo*t;
                yf = (v_limit-Canones.at(1)->getPosy()) + Vy0*t -(0.5*G*t*t);
                if(Impacto(xf,yf,10,YO,R)){                    
                    qDebug()<<xf<<yf<<10<<YO<<R;
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
                if(Impacto(xf,yf,Canones.at(0)->getPosx(),Canones.at(0)->getPosy(),Canones.at(2)->getR())){
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
}

void MainWindow::on_pushButton_6_clicked()
{
    ui->pushButton_6->hide();
    ui->splitter->show();
    ui->splitter_7->hide();
    ui->pushButton_7->show();

    if(Seed==1){
        Simulacion_1(Canones.at(1)->getPosx(),v_limit-Canones.at(1)->getPosy(),Canones.at(0)->getR());
        move->start(100);
    }
    else if(Seed==2){
        Simulacion_2(v_limit-Canones.at(0)->getPosy(),Canones.at(1)->getPosx(),v_limit-Canones.at(1)->getPosy(),Canones.at(2)->getR());
        move->start(100);
    }
    else if(Seed==3){
        Simulacion_3(ui->doubleSpinBox_2->value(),ui->doubleSpinBox->value());
        Disparos.push_back(new Proyectil_Graph(Canones.at(0)->getPosx(),v_limit-Canones.at(0)->getPosy(),ui->doubleSpinBox_2->value(),ui->doubleSpinBox->value(),Canones.at(1)->getPosx(),1));
        scene->addItem(Disparos.back());
        Disparos.push_back(new Proyectil_Graph(Canones.at(0)->getPosx(),v_limit-Canones.at(0)->getPosy(),ui->doubleSpinBox_2->value(),ui->doubleSpinBox->value(),Canones.at(1)->getPosx(),3));
        scene->addItem(Disparos.back());
        move->start(100);
    }
    else if(Seed==4){
        Simulacion_4(ui->doubleSpinBox_2->value(),ui->doubleSpinBox->value());
        Disparos.push_back(new Proyectil_Graph(Canones.at(0)->getPosx(),v_limit-Canones.at(0)->getPosy(),ui->doubleSpinBox_2->value(),ui->doubleSpinBox->value(),Canones.at(1)->getPosx(),1));
        scene->addItem(Disparos.back());
        Disparos.push_back(new Proyectil_Graph(Canones.at(0)->getPosx(),v_limit-Canones.at(0)->getPosy(),ui->doubleSpinBox_2->value(),ui->doubleSpinBox->value(),Canones.at(1)->getPosx(),3));
        scene->addItem(Disparos.back());
        move->start(100);
    }
}

void MainWindow::on_pushButton_7_clicked()
{
    move->stop();
    ui->pushButton_7->hide();
    scene->clear();
    Canones.clear();
    Disparos.clear();
}

void MainWindow::Actualizar()
{
    for(int i=0;i<Disparos.size();i++){
        Disparos.at(i)->actualizar(v_limit);
//        qDebug()<<Disparos.at(i)->getDisparo()->getPx()<<Disparos.at(i)->getDisparo()->getPy();
    }
}
