#include "rational.h"

Rational::Rational() {
  numerator_ = 0;
  denominator_ = 1;
}

Rational::Rational(int num) {
  numerator_ = num;
  denominator_ = 1;
}

Rational::Rational(int num1, int num2) {
  numerator_ = num1;
  SetDenominator(num2);
}

int Rational::GetNumerator() const { return numerator_; }

int Rational::GetDenominator() const { return denominator_; }

void Rational::ReduceFraction() {
  int gcd = std::gcd(numerator_, denominator_);
  int sign = 0;
  if (numerator_ * denominator_ >= 0) {
    sign = 1;
  } else {
    sign = -1;
  }
  numerator_ = std::abs(numerator_ / gcd) * sign;
  denominator_ = std::abs(denominator_ / gcd);
}

void Rational::SetNumerator(int num) {
  numerator_ = num;
  ReduceFraction();
}

void Rational::SetDenominator(int num) {
  if (num == 0) {
    throw RationalDivisionByZero();
  }

  denominator_ = num;
  ReduceFraction();
}

void Rational::SetWholeFraction(int num1, int num2) {
  numerator_ = num1;
  SetDenominator(num2);
}

Rational Rational::operator+(const Rational& obj) const {
  return {GetNumerator() * obj.GetDenominator() +
              obj.GetNumerator() * GetDenominator(),
          GetDenominator() * obj.GetDenominator()};
}

Rational Rational::operator-(const Rational& obj) const {
  return *this + Rational(-obj.GetNumerator(), obj.GetDenominator());
}

Rational Rational::operator/(const Rational& obj) const {
  return {GetNumerator() * obj.GetDenominator(),
          GetDenominator() * obj.GetNumerator()};
}

Rational Rational::operator*(const Rational& obj) const {
  return {GetNumerator() * obj.GetNumerator(),
          GetDenominator() * obj.GetDenominator()};
}

Rational& Rational::operator+=(const Rational& obj) {
  SetWholeFraction(GetNumerator() * obj.GetDenominator() +
                       GetDenominator() * obj.GetNumerator(),
                   GetDenominator() * obj.GetDenominator());
  return *this;
}

Rational& Rational::operator-=(const Rational& obj) {
  SetWholeFraction(GetNumerator() * obj.GetDenominator() -
                       GetDenominator() * obj.GetNumerator(),
                   GetDenominator() * obj.GetDenominator());
  return *this;
}

Rational& Rational::operator*=(const Rational& obj) {
  SetWholeFraction(GetNumerator() * obj.GetNumerator(),
                   GetDenominator() * obj.GetDenominator());
  return *this;
}

Rational& Rational::operator/=(const Rational& obj) {
  SetWholeFraction(GetNumerator() * obj.GetDenominator(),
                   GetDenominator() * obj.GetNumerator());
  return *this;
}

Rational Rational::operator+() const { return *this; }

Rational Rational::operator-() const {
  return {-GetNumerator(), GetDenominator()};
}

Rational& Rational::operator++() {
  SetNumerator(GetDenominator() + GetNumerator());
  return *this;
}

Rational& Rational::operator--() {
  SetNumerator(-GetDenominator() + GetNumerator());
  return *this;
}

Rational Rational::operator++(int) {
  Rational before = *this;
  SetNumerator(GetDenominator() + GetNumerator());
  return before;
}

Rational Rational::operator--(int) {
  Rational before = *this;
  SetNumerator(-GetDenominator() + GetNumerator());
  return before;
}

std::ostream& operator<<(std::ostream& stream, const Rational& obj) {
  stream << obj.GetNumerator();
  if (obj.GetDenominator() != 1) {
    stream << "/" << obj.GetDenominator();
  }

  return stream;
}

std::istream& operator>>(std::istream& stream, Rational& object) {
  std::string fraction;
  stream >> fraction;

  if (fraction.find('/') != std::string::npos) {
    auto chr = fraction.find('/');
    object.SetWholeFraction(
        std::stoi(fraction.substr(0, chr)),
        std::stoi(fraction.substr(chr + 1, fraction.length() - chr - 1)));
  } else {
    object.SetWholeFraction(std::stoi(fraction), 1);
  }

  return stream;
}