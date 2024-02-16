#include "MainWindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // Surcharger les fonctions resizeEvent
    // on récupère la largeur du tableau :
    resizeEvent();
}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resizeEvent(){
    ui->itemsWidget->width();
    ui->itemsWidget->setColumnWidth(0,ui->itemsWidget->width()/2);
    ui->itemsWidget->setColumnWidth(1,ui->itemsWidget->width()/4);
    ui->itemsWidget->setColumnWidth(2,ui->itemsWidget->width()/4);
}

void MainWindow::showEvent(){
    resizeEvent();
}
