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
        flag = true;
    }

    return flag;
}

void MainWindow::Simulacion_1(float XO,float YO, float XD, float YD, float R)
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
//                qDebug()<<Canones.at(0)->getPosy() + Vy0*t -(0.5*G*t*t)<<v_limit-Canones.at(0)->getPosy() + Vy0*t -(0.5*G*t*t)<<v_limit-(Canones.at(0)->getPosy() + Vy0*t -(0.5*G*t*t));
                yf = (v_limit-Canones.at(0)->getPosy()) + Vy0*t -(0.5*G*t*t);
                if(Impacto(xf,yf,XD,YD,R)){
                    qDebug()<<xf<<yf<<XD<<YD;
                    qDebug()<<R;
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



//    int flag = 0;
//    float x,y;
//    float Vx,Vy;
//    int VI0 = 0;
//    int t = 0;
//    int angle = 0;
//    for(VI0 = 0;VI0<500; VI0 += 5){
//        for(angle = 0; angle < 90; angle++){
//            Vx = VI0*cos(angle*pi/180);
//            Vy = VI0*sin(angle*pi/180);
//            x = 10;
//            y = ui->PYO->value();
//            for(t = 0; ; t++){
//                x = Vx*t;
//                y = YD + Vy*t -(0.5*G*t*t);
//                float Rang=XD*0.05;
//                if(Impacto(x,y,XD,YD,Rang)){
//                    Disparos.push_back(new Proyectil_Graph(10,ui->PYO->value(),angle,VI0,XD,1));
//                    scene->addItem(Disparos.back());
//                    flag += 1;
//                    VI0 += 50;
//                    break;
//                }
//                if(y < 0){
//                    break;
//                }
//            }
//            if(flag == 3) break;

//        }
//        if(flag == 3) break;
//    }
//    if(flag < 3){
//        msgBox.setText("No se encontraron al menos 3 disparos efectivos");

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
        Simulacion_1(Canones.at(0)->getPosx(),Canones.at(0)->getPosy(),Canones.at(1)->getPosx(),v_limit-Canones.at(1)->getPosy(),Canones.at(0)->getR());
        move->start(200);
    }
}

void MainWindow::on_pushButton_7_clicked()
{
    move->stop();
    ui->pushButton_7->hide();
    Canones.clear();
    scene->clear();
}

void MainWindow::Actualizar()
{
    for(int i=0;i<Disparos.size();i++){
        Disparos.at(i)->actualizar(v_limit);
//        qDebug()<<Disparos.at(i)->getDisparo()->getPx()<<Disparos.at(i)->getDisparo()->getPy();
    }
}
