#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>
#include "CalculatorModel.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT


public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


public slots:
    void emitIdClicked(int id);
    void executeCommand(int id);
    void updateBase(int index);
    void updateSuffix(bool checked);

signals:
    void idClicked(int id);

private:
    QLineEdit *_lineEdit;
    QLabel *_label;
    CalculatorModel *_calculatorModel;
    QComboBox *_comboBox;
    QVector<QPushButton*> _buttons;
    QAction *_actionSuffix;
    QWidget *_centralWidget;
    QVBoxLayout *_layout;

    void setupMenuBar();
    void setupCentralWidget();
    void setupButtons();
};
#endif // MAINWINDOW_H
