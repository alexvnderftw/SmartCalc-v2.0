#include "s21_controller_deposit.h"

namespace s21 {

// double ControllerDeposit::calculate(const std::string expr, const std::string
// x) {
// }

void ControllerDeposit::setDeposit(double value) { deposit_.setDeposit(value); }
void ControllerDeposit::setTerm(int term, Deposit::TermType unit) {
  deposit_.setTerm(term);
  deposit_.setTermType(unit);
}
void ControllerDeposit::setStartDate(int day, int month, int year) {
  deposit_.setStartDate(day, month, year);
}
void ControllerDeposit::setInterest(double value) {
  deposit_.setInterest(value);
}
void ControllerDeposit::setTax(double value) { deposit_.setTax(value); }
void ControllerDeposit::setCapitalization(bool set) {
  deposit_.setCapitalization(set);
}
void ControllerDeposit::setPeriodicity(Deposit::PayPeriod value) {
  deposit_.setPeriodicity(value);
}

void ControllerDeposit::addReplenish(Deposit::OperPeriod freq, Date date,
                                     double value) {
  deposit_.addReplenish(freq, date, value);
}
void ControllerDeposit::addWithdrawal(Deposit::OperPeriod freq, Date date,
                                      double value) {
  deposit_.addWithdrawal(freq, date, value);
}
void ControllerDeposit::removeReplenish(size_t index) {
  deposit_.removeReplenish(index);
}
void ControllerDeposit::removeWithdrawal(size_t index) {
  deposit_.removeWithdrawal(index);
}
size_t ControllerDeposit::getReplenishListSize() {
  return deposit_.getReplenishListSize();
}
size_t ControllerDeposit::getWIthdrawalListSize() {
  return deposit_.getWIthdrawalListSize();
}
size_t ControllerDeposit::getEventListSize() {
  return deposit_.getEventListSize();
}
std::vector<Deposit::Operation>::const_iterator
ControllerDeposit::getReplenishListElement(size_t index) {
  return deposit_.getReplenishListElement(index);
}
std::vector<Deposit::Operation>::const_iterator
ControllerDeposit::getWithdrawalListElement(size_t index) {
  return deposit_.getWithdrawalListElement(index);
}
std::vector<Deposit::Event>::const_iterator
ControllerDeposit::getEventListElement(size_t index) {
  return deposit_.getEventListElement(index);
}

void ControllerDeposit::calculate() { deposit_.calculate(); }

}  // namespace s21