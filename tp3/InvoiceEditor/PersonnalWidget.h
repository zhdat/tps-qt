#ifndef PERSONNALWIDGET_H
#define PERSONNALWIDGET_H

#include <QWidget>
#include <QPainter>
#include "InvoiceModel.h"

class PersonnalWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PersonnalWidget(QWidget *parent = nullptr);
    void setModeleDocument(InvoiceModel *modele);
signals:

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    InvoiceModel *invoiceModel;
};


#endif // PERSONNALWIDGET_H
