/**
 * @file   MainWidget.cpp
 * @author Sebastien Fourey
 */
#include "MainWidget.h"
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QString>
#include <QMouseEvent>
#include <QTimer>
#include <QTimeEdit>
#include "ColorWidget.h"
using namespace std;

MainWidget::MainWidget(QWidget * parent) : QWidget(parent)
{
  setGeometry(100, 100, 640, 480);

  QLabel * label = new QLabel("Colors, signals, and slots", this);
  label->setGeometry(10, 10, 290, 30);
  label->setFont(QFont("Arial", 14, QFont::Bold));

  _mouseValueDisplay = new QLineEdit(this);
  _mouseValueDisplay->setGeometry(200, 80, 300, 30);
  _mouseValueDisplay->setAlignment(Qt::AlignHCenter);
  _mouseValueDisplay->setReadOnly(true);

  _colorValueDisplay = new QLineEdit(this);
  _colorValueDisplay->setGeometry(200, 120, 300, 30);
  _colorValueDisplay->setAlignment(Qt::AlignHCenter);
  _colorValueDisplay->setReadOnly(true);

  ColorWidget * colorWidget = new ColorWidget(this);
  colorWidget->setGeometry(300, 160, 80, 30);

  QPushButton * pushButtonRandomColor = new QPushButton("Random color", this);
  pushButtonRandomColor->setGeometry(260, 200, 160, 30);

  connect(pushButtonRandomColor, &QPushButton::clicked, colorWidget, &ColorWidget::changeColor);

  connect(colorWidget, SIGNAL(colorChanged(int, int, int)), this, SLOT(onColorChanged(int, int, int)));

  QTimer *hours = new QTimer(this);
  hours->start(1);
  _displayHours = new QTimeEdit(QTime::currentTime(), this);
  _displayHours->setDisplayFormat("hh:mm:ss");
  _displayHours->setGeometry(260, 300, 160, 30);
  _displayHours->setReadOnly(true);

  connect(hours, &QTimer::timeout, this, &MainWidget::updateTimer);

  QPushButton * pushButtonQuit = new QPushButton("Quit", this);
  pushButtonQuit->setGeometry(260, 400, 160, 30);

  connect(pushButtonQuit, SIGNAL(clicked(bool)),
          this, SLOT(onQuitPressed()));
}

void MainWidget::onQuitPressed()
{
  int button = QMessageBox::question(this, "You want to quit...",
                                     "Are you sure that you want to quit"
                                     " this great application?",
                                     QMessageBox::Yes, QMessageBox::No | QMessageBox::Default);
  if (button == QMessageBox::Yes) {
    close();
  }
}

void MainWidget::onColorChanged(int r, int g, int b)
{
  QString text("Color: R(%1) G(%2) B(%3)");
  _colorValueDisplay->setText(text.arg(r).arg(g).arg(b));
}

void MainWidget::updateTimer(){
    _displayHours->setTime(QTime::currentTime());
}


void MainWidget::mouseMoveEvent(QMouseEvent * e){

    int x = e->x();
    int y = e->y();
    QString text("Position: x(%1) y(%2)");
    if(e->buttons()&Qt::LeftButton){
        _mouseValueDisplay->setAlignment(Qt::AlignLeft);
        _mouseValueDisplay->setText(text.arg(x).arg(y));
    }else if(e->buttons()&Qt::RightButton){
        _mouseValueDisplay->setAlignment(Qt::AlignRight);
        _mouseValueDisplay->setText(text.arg(x).arg(y));
    }else if(e->buttons()&Qt::MiddleButton){
        _mouseValueDisplay->setAlignment(Qt::AlignCenter);
        _mouseValueDisplay->setText(text.arg(x).arg(y));
    }

}


