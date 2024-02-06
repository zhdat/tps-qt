/** -*- mode: c++ ; c-basic-offset: 2 -*-
 * @file   InvoiceModel.cpp
 * @author Sebastien Fourey
 *
 * @brief  Invoice document model definition.
 */

#include "InvoiceModel.h"
#include <cmath>
#include <exception>
#include <limits>

InvoiceModel::InvoiceModel(QObject * parent) : QObject(parent)
{
  clear();
}

void InvoiceModel::setLastname(const QString & lastname)
{
  _lastname = lastname;
  sendNotificationIfEnabled();
}

void InvoiceModel::setFirstname(const QString & firstname)
{
  _firstname = firstname;
  sendNotificationIfEnabled();
}

void InvoiceModel::setAddressLine1(const QString & text)
{
  _addressLine1 = text;
  sendNotificationIfEnabled();
}

void InvoiceModel::setAddressLine2(const QString & text)
{
  _addressLine2 = text;
  sendNotificationIfEnabled();
}

void InvoiceModel::setZipcode(const QString & zipcode)
{
  _zipcode = zipcode;
  sendNotificationIfEnabled();
}

void InvoiceModel::setCity(const QString & city)
{
  _city = city;
  sendNotificationIfEnabled();
}

void InvoiceModel::setCell(int row, int column, const QString & text)
{
  if (row < 0 || row >= ROWS) {
    throw std::out_of_range("bad row index");
  }
  if (column < 0 || column > 2) {
    throw std::out_of_range("bad column index");
  }
  switch (column) {
  case 0:
    _description[row] = text;
    sendNotificationIfEnabled();
    break;
  case 1:
    _unitPrice[row] = text2float(text);
    updateRow(row);
    break;
  case 2:
    _quantity[row] = text2float(text);
    updateRow(row);
  }
}

void InvoiceModel::setVATRate(float rate)
{
  _VATRate = rate;
}

void InvoiceModel::clear()
{
  _lastname.clear();
  _firstname.clear();
  _addressLine1.clear();
  _addressLine2.clear();
  _zipcode.clear();
  _city.clear();
  for (int row = 0; row < ROWS; ++row) {
    _description[row].clear();
    _unitPrice[row] = 0.0f;
    _quantity[row] = 0.0f;
    _price[row] = 0.0f;
  }
  updateTotal();
}

void InvoiceModel::sendNotificationIfEnabled()
{
  if (_notify) {
    emit notify();
  }
}

void InvoiceModel::updateRow(int row)
{
  if (std::isnan(_unitPrice[row]) || std::isnan(_quantity[row])) {
    _price[row] = NAN;
  } else {
    _price[row] = _unitPrice[row] * _quantity[row];
  }
  updateTotal();
}

void InvoiceModel::updateTotal()
{
  _totalWithoutVAT = 0.0f;
  for (int row = 0; row < ROWS; ++row) {
    _totalWithoutVAT += std::isnan(_price[row]) ? 0.0f : _price[row];
  }
  sendNotificationIfEnabled();
}

float InvoiceModel::text2float(const QString & text)
{
  if (text.trimmed().isEmpty()) {
    return 0.0f;
  }
  bool ok;
  float x = text.toFloat(&ok);
  if (!ok) {
    x = NAN;
  }
  return x;
}

const QString & InvoiceModel::lastname() const
{
  return _lastname;
}

const QString & InvoiceModel::firstname() const
{
  return _firstname;
}

const QString & InvoiceModel::addressLine1() const
{
  return _addressLine1;
}

const QString & InvoiceModel::addressLine2() const
{
  return _addressLine2;
}

const QString & InvoiceModel::zipcode() const
{
  return _zipcode;
}

const QString & InvoiceModel::city() const
{
  return _city;
}

const QString InvoiceModel::cell(int row, int column) const
{
  if (row < 0 || row >= ROWS) {
    throw std::out_of_range("bad row index");
  }
  if (column < 0 || column > 3) {
    throw std::out_of_range("bad column index");
  }
  switch (column) {
  case 1:
    return amountToString(_unitPrice[row]);
  case 2:
    return std::isnan(_quantity[row]) ? tr("ERROR") : QString::number((double)_quantity[row], 'g');
  case 3:
    return amountToString(_price[row]);
  case 0: // Description
  default:
    return _description[row];
  }
}

bool InvoiceModel::rowIsEmpty(int row) const
{
  return _description[row].isEmpty() && _unitPrice[row] == 0.0f && _quantity[row] == 0.0f;
}

float InvoiceModel::totalWithoutTaxes() const
{
  return _totalWithoutVAT;
}

float InvoiceModel::VAT() const
{
  return _totalWithoutVAT * (_VATRate / 100.0f);
}

float InvoiceModel::totalWithTaxes() const
{
  return totalWithoutTaxes() + VAT();
}

int InvoiceModel::decimals() const
{
  return _decimals;
}

void InvoiceModel::setDecimals(int decimals)
{
  _decimals = decimals;
}

QString InvoiceModel::amountToString(float value) const
{
  if (std::isnan(value)) {
    return tr("ERROR");
  }
  return QString::number((double)value, 'f', _decimals);
}

void InvoiceModel::enableNotifications()
{
  _notify = true;
}

void InvoiceModel::disableNotifications()
{
  _notify = false;
}
