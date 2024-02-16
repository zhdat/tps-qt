#include "MainWindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QStringList headers;
    headers << "Produits" << "Prix à l'unité" << "Quantité";
    ui->itemsWidget->setHorizontalHeaderLabels(headers);

    model = new InvoiceModel(this);
    ui->InvoiceView->setModel(model);
    connect(ui->lastnameEdit, &QLineEdit::textChanged, this, &MainWindow::on_lastnameEdit_textChanged);
    connect(ui->firstnameEdit, &QLineEdit::textChanged, this, &MainWindow::on_firstnameEdit_textChanged);
}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    ui->itemsWidget->setColumnWidth(0,ui->itemsWidget->viewport()->width()/2);
    ui->itemsWidget->setColumnWidth(1,ui->itemsWidget->viewport()->width()/4);
    ui->itemsWidget->setColumnWidth(2,ui->itemsWidget->viewport()->width()/4);
}


void MainWindow::showEvent(QShowEvent *event){
    ui->itemsWidget->setColumnWidth(0,ui->itemsWidget->viewport()->width()/2);
    ui->itemsWidget->setColumnWidth(1,ui->itemsWidget->viewport()->width()/4);
    ui->itemsWidget->setColumnWidth(2,ui->itemsWidget->viewport()->width()/4);
}

void MainWindow::on_lastnameEdit_textChanged(const QString &text){
    if(model){
        model->setLastname(text);
    }
}

void MainWindow::on_firstnameEdit_textChanged(const QString &text){
    if(model){
        model->setFirstname(text);
    }
}
