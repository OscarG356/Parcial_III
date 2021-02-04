#include "mainwindow.h"
#include "ui_mainwindow.h"

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
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Preparar_Simulacion()
{
    Canones.push_back(new Canon(15,v_limit-ui->PYO->value(),1));
    scene->addItem(Canones.back());
    Canones.push_back(new Canon(ui->PXD->value(),v_limit-ui->PYD->value(),2));
    scene->addItem(Canones.back());
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
}

void MainWindow::on_pushButton_4_clicked()
{
    msgBox.setText("Dado un disparo ofensivo, generar disparo defensivos que impidan que los cañones defensivo"
                   "y ofensivo puedan ser destruidos.");
    msgBox.exec();
    Preparar_Simulacion();
    ui->pushButton_6->show();
    Seed=4;
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
}
