#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "InvoiceModel.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void resizeEvent(QResizeEvent *event) override;
    void showEvent(QShowEvent *event) override;


public slots:
    void on_lastnameEdit_textChanged(const QString &text);
    void on_firstnameEdit_textChanged(const QString &text);

private:
    Ui::MainWindow *ui;
    InvoiceModel *model;
};
#endif // MAINWINDOW_H
