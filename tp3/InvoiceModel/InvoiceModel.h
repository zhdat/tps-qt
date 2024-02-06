/** -*- mode: c++ ; c-basic-offset: 2 -*-
 * @file   InvoiceModel.h
 * @author Sebastien Fourey
 *
 * @brief  Invoice document model declaration.
 */
#ifndef INVOICEMODEL_H
#define INVOICEMODEL_H
#include <QObject>
#include <QString>
#include <iostream>

class InvoiceModel : public QObject {

  Q_OBJECT

public:
  explicit InvoiceModel(QObject * parent = nullptr);

  const QString & lastname() const;
  const QString & firstname() const;
  const QString & addressLine1() const;
  const QString & addressLine2() const;
  const QString & zipcode() const;
  const QString & city() const;
  /*
   * @param row in [0..14]
   * @param col in [0..3]
   */
  const QString cell(int row, int col) const;
  bool rowIsEmpty(int row) const;
  float totalWithoutTaxes() const;
  float VAT() const;
  float totalWithTaxes() const;
  static inline int rows();
  static inline int columns();
  int decimals() const;
  void setDecimals(int decimals);
  QString amountToString(float value) const;
  void sendNotificationIfEnabled();

public slots:
  void enableNotifications();
  void disableNotifications();
  void setLastname(const QString & lastname);
  void setFirstname(const QString & firstname);
  void setAddressLine1(const QString & text);
  void setAddressLine2(const QString & text);
  void setZipcode(const QString & zipcode);
  void setCity(const QString & city);

  /*
   * @param row in [0..14]
   * @param col in [0..2] (price per item is computed)
   */
  void setCell(int row, int column, const QString & text);
  void setVATRate(float rate);
  inline float VATRate() const;
  void clear();

signals:
  void notify();

private:
  void updateRow(int row);
  void updateTotal();
  static float text2float(const QString & text);
  QString _lastname;
  QString _firstname;
  QString _addressLine1;
  QString _addressLine2;
  QString _zipcode;
  QString _city;
  static const int ROWS = 15;
  QString _description[ROWS];
  float _unitPrice[ROWS];
  float _quantity[ROWS];
  float _price[ROWS];
  float _VATRate = 18.6f;
  float _totalWithoutVAT = 0.0f;
  bool _notify = true;
  int _decimals = 2;
};

// Inline methods

int InvoiceModel::rows()
{
  return ROWS;
}

int InvoiceModel::columns()
{
  return 3;
}

float InvoiceModel::VATRate() const
{
  return _VATRate;
}

#endif // defined(INVOICEMODEL_H)
