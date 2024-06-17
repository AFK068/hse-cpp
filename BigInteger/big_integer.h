#ifndef HSE_BIG_INTEGER_H
#define HSE_BIG_INTEGER_H

#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

class BigIntegerOverflow : public std::runtime_error {
 public:
  BigIntegerOverflow() : std::runtime_error("BigIntegerOverflow") {}
};

class BigIntegerDivisionByZero : public std::runtime_error {
 public:
  BigIntegerDivisionByZero() : std::runtime_error("BigIntegerDivisionByZero") {}
};

class BigInteger {
 private:
  int sign_ = 1;
  std::vector<std::string> number_;

 public:
  BigInteger() = default;
  BigInteger(const char*);                                         // NOLINT
  BigInteger(int64_t);                                             // NOLINT
  BigInteger(int num) : BigInteger(static_cast<int64_t>(num)) {};  // NOLINT

  bool IsNegative() const;

  BigInteger ChangeSignToZeroNumber(const BigInteger&) const;
  std::vector<std::string> RemoveLeadingZeros(
      const std::vector<std::string>&) const;

  bool ComparisonForNegativeNumbers(const BigInteger&, const BigInteger&) const;
  std::vector<std::string> ParseStringToVector(std::string&, size_t);
  std::vector<std::string> ParseNumberToVector(int64_t, size_t);
  std::vector<std::string> AddZeros(std::vector<int>&) const;
  void SubstringNegativeNumber(std::vector<std::string>&);

  friend bool operator<(const BigInteger&, const BigInteger&);
  friend bool operator>(const BigInteger&, const BigInteger&);
  friend bool operator<=(const BigInteger&, const BigInteger&);
  friend bool operator>=(const BigInteger&, const BigInteger&);

  BigInteger operator+() const;
  BigInteger operator-() const;

  BigInteger& operator+=(const BigInteger&);
  BigInteger operator+(const BigInteger&) const;

  BigInteger operator-(const BigInteger&) const;
  BigInteger& operator-=(const BigInteger&);

  BigInteger operator*(const BigInteger&) const;
  BigInteger& operator*=(const BigInteger&);

  // BigInteger operator/(const BigInteger&);
  // BigInteger& operator/=(const BigInteger&);

  BigInteger& operator++();
  const BigInteger operator++(int);

  BigInteger& operator--();
  const BigInteger operator--(int);

  explicit operator bool() const;

  bool operator==(const BigInteger&) const;
  bool operator!=(const BigInteger&) const;

  friend std::ostream& operator<<(std::ostream&, const BigInteger&);
  friend std::istream& operator>>(std::istream&, BigInteger&);
};

#endif
