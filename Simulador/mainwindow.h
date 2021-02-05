#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <fisica_proyectil.h>
#include <proyectil_graph.h>
#include <QGraphicsScene>
#include <QMainWindow>
#include <QMessageBox>
#include <canon.h>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void Preparar_Simulacion();
    bool Impacto(float XO,float YO,float XD,float YD,float Rang);
    void Simulacion_1(float XO,float YO, float XD, float YD, float R);

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_5_clicked();
    void on_pushButton_6_clicked();
    void on_pushButton_7_clicked();

public slots:
    void Actualizar();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene = new QGraphicsScene;

    int v_limit, h_limit;
    int Seed;

    QTimer *move;

    QMessageBox msgBox;

    QList<Canon *>Canones;
    QList<Proyectil_Graph *>Disparos;

};
#endif // MAINWINDOW_H
