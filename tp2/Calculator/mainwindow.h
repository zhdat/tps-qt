#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
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
};
#endif // MAINWINDOW_H
