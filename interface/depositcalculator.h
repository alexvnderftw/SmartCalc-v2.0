#ifndef DEPOSITCALCULATOR_H
#define DEPOSITCALCULATOR_H

#include <QWidget>
#include "../controller/s21_controller_deposit.h"

namespace Ui {
class DepositCalculator;
}

class DepositCalculator : public QWidget
{
    Q_OBJECT

public:
    explicit DepositCalculator(QWidget *parent = nullptr);
    ~DepositCalculator();

    void setDefaultFocus();

private slots:
    void calculate();
    void addReplenish();
    void removeReplenish();
    void addWithdrawal();
    void removeWithdrawal();
    void setCapitalization(int value);

private:
    Ui::DepositCalculator *ui;
    s21::ControllerDeposit data;

    void connectSignals();

    void setCurrentDate();
    void setValues();
    s21::Deposit::TermType getUnitType();
    s21::Deposit::PayPeriod getPayPeriod();
    s21::Deposit::OperPeriod getOperPeriod();

    void fillTable();
    void fillTaxes();
    void fillLines();
    void clearContent();

    QString getDateString(s21::Date date);
    QString getEventString(s21::Deposit::EventType event);
    QString getOperPeriodString(s21::Deposit::OperPeriod period);

};

#endif // DEPOSITCALCULATOR_H
