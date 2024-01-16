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
#include "ColorWidget.h"
using namespace std;

MainWidget::MainWidget(QWidget * parent) : QWidget(parent)
{
  setGeometry(100, 100, 640, 480);

  QLabel * label = new QLabel("Colors, signals, and slots", this);
  label->setGeometry(10, 10, 290, 30);
  label->setFont(QFont("Arial", 14, QFont::Bold));

  _colorValueDisplay = new QLineEdit(this);
  _colorValueDisplay->setGeometry(200, 120, 300, 30);
  _colorValueDisplay->setAlignment(Qt::AlignHCenter);

  ColorWidget * colorWidget = new ColorWidget(this);
  colorWidget->setGeometry(300, 160, 80, 30);

  QPushButton * pushButtonRandomColor = new QPushButton("Random color", this);
  pushButtonRandomColor->setGeometry(260, 200, 160, 30);

  connect(pushButtonRandomColor, &QPushButton::clicked, colorWidget, &ColorWidget::changeColor);

  connect(colorWidget, SIGNAL(colorChanged(int, int, int)), this, SLOT(onColorChanged(int, int, int)));
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
