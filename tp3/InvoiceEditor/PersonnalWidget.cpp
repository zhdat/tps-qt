#include "PersonnalWidget.h"

PersonnalWidget::PersonnalWidget(QWidget *parent)
    : QWidget{parent}, invoiceModel(nullptr)
{

}


void PersonnalWidget::setModel(InvoiceModel *modele)
{
    invoiceModel = modele;
    connect(invoiceModel, &InvoiceModel::notify, this, &PersonnalWidget::updateView);
    update(); // Rafraîchir le widget dès que le modèle est associé
}

void PersonnalWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);

        // Dessiner un fond blanc
    painter.fillRect(rect(), Qt::white);

        // Dessiner la bordure
    painter.setPen(QPen(Qt::black, 2));
    painter.drawRect(rect().adjusted(0, 0, -1, -1)); // ajuster la taille du rectangle pour éviter de peindre sur la bordure

    if (true) {
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setPen(QPen(Qt::black, 1));

        QRect roundedRect(5, 5, 200, 100);
        painter.setBrush(Qt::lightGray);
        painter.drawRoundedRect(roundedRect, 20, 20);

                // Dessiner un tableau à 4 colonnes et 2 lignes
        int tableWidth = width() - 10 ; // largeur disponible pour le tableau
        int tableHeight = height() - 150; // la moitié de la hauteur disponible pour le tableau
        QFontMetrics fm(painter.font());
        int textHeight = fm.height();
        int cellHeightFirst = textHeight; // hauteur de chaque cellule
        int cellHeight = tableHeight - cellHeightFirst -150;


                // Dessiner les lignes et colonnes du tableau
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 2; j++) {
                if (j == 0){
                    if (i == 0){
                        QRect cellRect( 5+ i * 2 * (tableWidth / 5), 150 + j * cellHeightFirst, 2* tableWidth / 5, cellHeightFirst);
                        painter.drawRect(cellRect);
                    }else {
                        QRect cellRect( 5+ (1+i) * (tableWidth / 5), 150 + j * cellHeightFirst, tableWidth / 5, cellHeightFirst);
                        painter.drawRect(cellRect);
                    }

                }
                else {
                    if (i == 0){
                        QRect cellRect( 5+ i * 2 * (tableWidth / 5), 150 + j * cellHeightFirst, 2*tableWidth / 5, cellHeight);
                        painter.drawRect(cellRect);
                    }else {
                        QRect cellRect( 5+ (1+i) * (tableWidth / 5), 150 + j * cellHeightFirst, tableWidth / 5, cellHeight);
                        painter.drawRect(cellRect);
                    }

                }

            }
        }
        painter.drawText(QRect(5, 150, 2* tableWidth / 5, cellHeight), Qt::AlignLeft, "Produit");
        painter.drawText(QRect(5 + 2 * tableWidth / 5, 150, tableWidth / 5, cellHeight), Qt::AlignLeft, "Prix à l'unité");
        painter.drawText(QRect(5 + 3 * tableWidth / 5, 150, tableWidth / 5, cellHeight), Qt::AlignLeft, "Quantité");
        painter.drawText(QRect(5 + 4 * tableWidth / 5, 150, tableWidth / 5, cellHeight), Qt::AlignLeft, "Prix");

        int textWidth = fm.width("Total");
        QRect vatRect(width() - textWidth - 150, tableHeight + 30, textWidth, textHeight);
        QRect totalRect(width() - textWidth - 150, tableHeight + 60, textWidth, textHeight);
        painter.drawText(vatRect, "VAT");
        painter.drawText(totalRect, "Total");
        painter.drawRect(QRect(width() - textWidth - 90, tableHeight + 30, 90, textHeight));
        painter.drawRect(QRect(width() - textWidth - 90, tableHeight + 60, 90, textHeight));
     }
    if(invoiceModel){
        QFontMetrics fm(painter.font());
        painter.drawText(QRect(15, 15, 200, 100), Qt::AlignLeft, invoiceModel->lastname());
        int lastnameWidth = fm.width(invoiceModel->lastname());
        painter.drawText(QRect(15 + lastnameWidth + 5, 15, 200, 100), Qt::AlignLeft, invoiceModel->firstname());
    }
}

void PersonnalWidget::updateView(){
    update();
}

