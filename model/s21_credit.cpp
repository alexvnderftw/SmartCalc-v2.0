#include "s21_credit.h"

namespace s21 {

void Credit::SetAnnuity() noexcept { payment_type_ = ANNUITY; }
void Credit::SetDifferential() noexcept { payment_type_ = DIFFERENTIAL; }
void Credit::SetTime(int months) noexcept { time_ = months; }
void Credit::SetRate(double rate) noexcept { rate_ = rate; }
void Credit::SetCredit(double credit) noexcept { credit_ = credit; }
void Credit::SetStartDate(int month, int year) {
  start_month_ = month;
  start_year_ = year;
}
void Credit::SetStartDate() {
  time_t t = std::time(NULL);
  struct tm time_data = *std::localtime(&t);
  start_month_ = time_data.tm_mon + 1;
  start_year_ = time_data.tm_year + 1900;
}

Credit::Type Credit::GetType() const noexcept { return payment_type_; }
int Credit::GetTime() const noexcept { return time_; }
double Credit::GetRate() const noexcept { return rate_; }
double Credit::GetCredit() const noexcept { return credit_; }
int Credit::GetStartMonth() const noexcept { return start_month_; }
int Credit::GetStartYear() const noexcept { return start_year_; }
double Credit::GetSummaryPaid() const noexcept { return sum_paid_; }
double Credit::GetSummaryMainPart() const noexcept { return sum_main_paid_; }
double Credit::GetSummaryRatePart() const noexcept { return sum_rate_paid_; }
size_t Credit::GetDataSize() const noexcept { return data_.size(); }
std::vector<Credit::CreditMonth>::const_iterator Credit::GetData()
    const noexcept {
  return data_.cbegin();
}
Credit::CreditMonth Credit::operator[](size_t pos) const noexcept {
  return data_[pos];
}

/* Main function. Call it after setting all values.
If all data correct it fills data vector and summary, and returns true.
If some data incorrect returns false. */
bool Credit::Calculate() {
  if (Validate() == true) {
    Reset();
    data_.resize(time_);
    data_.shrink_to_fit();
    if (payment_type_ == ANNUITY)
      CalculateAnnuity();
    else
      CalculateDifferential();
    return true;
  }
  return false;
}

/* Misc */

void Credit::Reset() noexcept {
  sum_paid_ = 0.0;
  sum_main_paid_ = 0.0;
  sum_rate_paid_ = 0.0;
  data_.clear();
}

bool Credit::Validate() const noexcept {
  return ValidateCredit() && ValidateRate() && ValidateTime() &&
         ValidateStartDate();
}

bool Credit::ValidateCredit() const noexcept {
  return credit_ > 0.0 && credit_ <= MAX_CREDIT_ && !std::isnan(credit_) &&
         !std::isinf(credit_);
}

bool Credit::ValidateRate() const noexcept {
  return rate_ > 0.0 && rate_ <= MAX_RATE_ && !std::isnan(rate_) &&
         !std::isinf(rate_);
}

bool Credit::ValidateTime() const noexcept {
  return time_ > 0 && time_ <= MAX_TIME_;
}

bool Credit::ValidateStartDate() const noexcept {
  return start_month_ > 0 && start_month_ <= 12 && start_year_ >= MIN_YEAR_ &&
         start_year_ <= MAX_YEAR_;
}

void Credit::CalculateAnnuity() noexcept {
  data_[0].month_ = start_month_;
  data_[0].year_ = start_year_;
  data_[0].rate_fee_ = bankRoundTwoDecimal(credit_ * rate_ / 12.0);
  data_[0].payment_ = bankRoundTwoDecimal(
      credit_ * rate_ / 12.0 /
      (1.0 - std::pow(1.0 + rate_ / 12.0, -static_cast<double>(time_))));
  data_[0].main_fee_ =
      bankRoundTwoDecimal(data_[0].payment_ - data_[0].rate_fee_);
  data_[0].leftover_ = bankRoundTwoDecimal(credit_ - data_[0].main_fee_);

  for (int i = 1; i < time_; ++i) {
    data_[i].month_ = data_[i - 1].month_ + 1;
    data_[i].year_ = data_[i - 1].year_;
    if (data_[i].month_ > 12) {
      data_[i].month_ = 1;
      data_[i].year_++;
    }
    data_[i].payment_ = data_[0].payment_;
    data_[i].rate_fee_ =
        bankRoundTwoDecimal(data_[i - 1].leftover_ * rate_ / 12.0);
    data_[i].main_fee_ =
        bankRoundTwoDecimal(data_[i].payment_ - data_[i].rate_fee_);
    data_[i].leftover_ =
        bankRoundTwoDecimal(data_[i - 1].leftover_ - data_[i].main_fee_);
  }
  CalculateSummary();
}

void Credit::CalculateDifferential() noexcept {
  data_[0].month_ = start_month_;
  data_[0].year_ = start_year_;
  data_[0].main_fee_ = credit_ / time_;
  data_[0].payment_ = data_[0].main_fee_ + credit_ * rate_ / 12.0;
  data_[0].rate_fee_ = data_[0].payment_ - data_[0].main_fee_;
  data_[0].leftover_ = credit_ - data_[0].main_fee_;

  for (int i = 1; i < time_; ++i) {
    data_[i].month_ = data_[i - 1].month_ + 1;
    data_[i].year_ = data_[i - 1].year_;
    if (data_[i].month_ > 12) {
      data_[i].month_ = 1;
      data_[i].year_++;
    }
    data_[i].main_fee_ = data_[0].main_fee_;
    data_[i].payment_ =
        data_[0].main_fee_ + (credit_ - data_[0].main_fee_ * i) * rate_ / 12.0;
    data_[i].rate_fee_ = data_[i].payment_ - data_[i].main_fee_;
    data_[i].leftover_ = data_[i - 1].leftover_ - data_[i].main_fee_;
  }
  CalculateSummary();
  RoundData();
}

void Credit::CalculateSummary() noexcept {
  for (int i = 0; i < time_; ++i) {
    sum_paid_ += data_[i].payment_;
    sum_main_paid_ += data_[i].main_fee_;
    sum_rate_paid_ += data_[i].rate_fee_;
  }
}

void Credit::RoundData() noexcept {
  for (int i = 0; i < time_; ++i) {
    data_[i].main_fee_ = bankRoundTwoDecimal(data_[i].main_fee_);
    data_[i].payment_ = bankRoundTwoDecimal(data_[i].payment_);
    data_[i].rate_fee_ = bankRoundTwoDecimal(data_[i].rate_fee_);
    data_[i].leftover_ = bankRoundTwoDecimal(data_[i].leftover_);
  }
  sum_paid_ = bankRoundTwoDecimal(sum_paid_);
  sum_main_paid_ = bankRoundTwoDecimal(sum_main_paid_);
  sum_rate_paid_ = bankRoundTwoDecimal(sum_rate_paid_);
}

}  // namespace s21
