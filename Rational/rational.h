#ifndef H_RATIONAL
#define H_RATIONAL

#include <algorithm>
#include <iostream>
#include <numeric>
#include <stdexcept>

class RationalDivisionByZero : std::runtime_error {
 public:
  RationalDivisionByZero() : std::runtime_error("RationalDivisionByZero") {}
};

class Rational {
 private:
  int numerator_;
  int denominator_;

 public:
  Rational();
  Rational(int);  // NOLINT
  Rational(int, int);

  int GetNumerator() const;
  int GetDenominator() const;

  void ReduceFraction();

  void SetNumerator(int);
  void SetDenominator(int);
  void SetWholeFraction(int, int);

  Rational operator+(const Rational&) const;
  Rational operator-(const Rational&) const;
  Rational operator*(const Rational&) const;
  Rational operator/(const Rational&) const;

  Rational& operator+=(const Rational&);
  Rational& operator-=(const Rational&);
  Rational& operator/=(const Rational&);
  Rational& operator*=(const Rational&);

  Rational operator+() const;
  Rational operator-() const;

  Rational& operator++();
  Rational& operator--();
  Rational operator++(int);
  Rational operator--(int);

  friend std::ostream& operator<<(std::ostream&, const Rational&);
  friend std::istream& operator>>(std::istream&, Rational&);
};

inline bool operator==(const Rational& r1, const Rational& r2) {
  return (r1.GetDenominator() == r2.GetDenominator() &&
          r1.GetNumerator() == r2.GetNumerator());
}

inline bool operator>(const Rational& r1, const Rational& r2) {
  return (r1.GetNumerator() * r2.GetDenominator() > r2.GetNumerator() *
          r1.GetDenominator());
}

inline bool operator<(const Rational& r1, const Rational& r2) {
  return r2 > r1;
}

inline bool operator>=(const Rational& r1, const Rational& r2) {
  return !(r1 < r2);
}

inline bool operator<=(const Rational& r1, const Rational& r2) {
  return !(r1 > r2);
}

#endif