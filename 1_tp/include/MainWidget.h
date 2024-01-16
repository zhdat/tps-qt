/**
 * @file   MainWidget.h
 * @author Sebastien Fourey
 *
 * @brief  Class of the application's main Widget .
 */
#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <iostream>

class QLineEdit;

/**
 * Class of the application's main Widget .
 */
class MainWidget : public QWidget {

  // Tell "moc" that this is a signal sender or receiver.
  Q_OBJECT

public:
  /**
   * Default constructor.
   */
  MainWidget(QWidget * parent = nullptr);

public slots:

  /**
   * Action when the "Quit" button is clicked.
   * (Note : the button does not exist yet.)
   */
  void onQuitPressed();

  /**
   * Action when color changes in the ColorWidget.
   *
   * @param r The red component in [0..255].
   * @param g The green component in [0..255].
   * @param b The blue component in [0..255].
   */
  void onColorChanged(int r, int g, int b);

private:
  QLineEdit * _colorValueDisplay; /**< A text field */
};

#endif
