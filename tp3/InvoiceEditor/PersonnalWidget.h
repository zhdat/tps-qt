#ifndef PERSONNALWIDGET_H
#define PERSONNALWIDGET_H

#include <QWidget>
#include <QPainter>

class PersonnalWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PersonnalWidget(QWidget *parent = nullptr);

signals:

protected:
    void paintEvent(QPaintEvent *event) override {
            Q_UNUSED(event);

            QPainter painter(this);
            painter.setRenderHint(QPainter::Antialiasing);

            // Définir la couleur de remplissage et de contour
            painter.setBrush(QColor("#f0f0f0"));
            painter.setPen(QPen(Qt::black, 2));

            // Dessiner le rectangle avec des bords arrondis
            int radius = 20;
            painter.drawRoundedRect(rect().adjusted(2, 2, -2, -2), radius, radius);
        }

};

#endif // PERSONNALWIDGET_H
