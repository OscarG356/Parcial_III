#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <fisica_proyectil.h>
#include <proyectil_graph.h>
#include <QGraphicsScene>
#include <QMainWindow>
#include <QMessageBox>
#include <canon.h>

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

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_5_clicked();
    void on_pushButton_6_clicked();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene = new QGraphicsScene;

    int v_limit, h_limit;
    int Seed;

    QMessageBox msgBox;

    QList<Canon *>Canones;
    QList<Proyectil_Graph *>Disparos;

};
#endif // MAINWINDOW_H
