#include "big_integer.h"

BigInteger BigInteger::ChangeSignToZeroNumber(const BigInteger& integer) const {
  bool all_zeros = false;
  BigInteger temp = integer;

  for (size_t i = 0; i < integer.number_.size(); ++i) {
    if (std::stoi(integer.number_[i]) != 0) {
      break;
    }

    all_zeros = true;
  }

  if (all_zeros) {
    temp.sign_ = 1;
  }

  return temp;
}

BigInteger::BigInteger(const char* str) {
  std::string char_to_string_with_sign = std::string(str + 1);
  std::string char_to_string = std::string(str);

  if (str[0] == '-') {
    sign_ *= -1;
    number_ = ParseStringToVector(char_to_string_with_sign, 5);
  } else if (str[0] == '+') {
    number_ = ParseStringToVector(char_to_string_with_sign, 5);
  } else {
    number_ = ParseStringToVector(char_to_string, 5);
  }

  if (number_.size() > 10000) {
    throw BigIntegerOverflow();
  }
}

BigInteger::BigInteger(int64_t num) {
  if (num < 0) {
    sign_ = -1;
    num = -num;
  } else {
    sign_ = 1;
  }

  number_ = ParseNumberToVector(num, 5);
}

bool BigInteger::IsNegative() const { return sign_ != 1; }

std::vector<std::string> BigInteger::ParseStringToVector(std::string& str,
                                                         size_t block_size) {
  std::vector<std::string> result;
  std::reverse(str.begin(), str.end());

  for (size_t i = 0; i < str.size(); i += block_size) {
    std::string sub_string = str.substr(i, block_size);
    std::reverse(sub_string.begin(), sub_string.end());
    result.push_back(sub_string);
  }

  std::reverse(result.begin(), result.end());
  return result;
}

std::vector<std::string> BigInteger::ParseNumberToVector(int64_t number,
                                                         size_t block_size) {
  std::string str = std::to_string(number);

  std::vector<std::string> result;
  std::reverse(str.begin(), str.end());

  for (size_t i = 0; i < str.size(); i += block_size) {
    std::string sub_string = str.substr(i, block_size);
    std::reverse(sub_string.begin(), sub_string.end());
    result.push_back(sub_string);
  }

  std::reverse(result.begin(), result.end());
  return result;
}

std::vector<std::string> BigInteger::AddZeros(std::vector<int>& result) const {
  std::vector<std::string> formatted_result;
  std::reverse(result.begin(), result.end());

  for (size_t i = 0; i < result.size(); ++i) {
    std::string num_str = std::to_string(result[i]);

    if (i != 0 && result[i] == 0) {
      num_str = "00000";
    } else if (i != 0) {
      while (num_str.length() < 5) {
        num_str = "0" + num_str;
      }
    }

    formatted_result.push_back(num_str);
  }

  return formatted_result;
}

BigInteger BigInteger::operator+() const { return *this; }

BigInteger BigInteger::operator-() const {
  BigInteger result = *this;
  if (!result.number_.empty() && result.number_[0] != "0") {
    result.sign_ *= -1;
  }

  return result;
}

bool BigInteger::ComparisonForNegativeNumbers(
    const BigInteger& integer1, const BigInteger& integer2) const {
  if (integer1.sign_ == -1 && integer2.sign_ == -1) {
    BigInteger temp1 = integer1;
    temp1.sign_ = 1;

    BigInteger temp2 = integer2;
    temp2.sign_ = 1;

    return (temp1 < temp2);
  }

  if (integer1.number_.size() != integer2.number_.size()) {
    return integer1.number_.size() < integer2.number_.size();
  }

  for (size_t i = 0; i < integer1.number_.size(); ++i) {
    if (integer1.number_[i] != integer2.number_[i]) {
      return std::stoi(integer1.number_[i]) < std::stoi(integer2.number_[i]);
    }
  }

  return false;
}

bool operator<(const BigInteger& integer1, const BigInteger& integer2) {
  if (integer1.sign_ != integer2.sign_) {
    return integer1.sign_ < integer2.sign_;
  }

  if (integer1.number_.size() != integer2.number_.size()) {
    return integer1.sign_ == 1
               ? integer1.number_.size() < integer2.number_.size()
               : integer1.number_.size() > integer2.number_.size();
  }

  for (size_t i = 0; i < integer1.number_.size(); ++i) {
    if (integer1.number_[i] != integer2.number_[i]) {
      return integer1.sign_ == 1 ? std::stoi(integer1.number_[i]) <
                                       std::stoi(integer2.number_[i])
                                 : std::stoi(integer1.number_[i]) >
                                       std::stoi(integer2.number_[i]);
    }
  }

  return false;
}

bool operator>(const BigInteger& integer1, const BigInteger& integer2) {
  return integer2 < integer1;
}

bool operator<=(const BigInteger& integer1, const BigInteger& integer2) {
  return !(integer1 > integer2);
}

bool operator>=(const BigInteger& integer1, const BigInteger& integer2) {
  return !(integer1 < integer2);
}

BigInteger BigInteger::operator+(const BigInteger& integer) const {
  BigInteger temp1 = *this;
  BigInteger temp2 = integer;

  if (temp1.number_.size() > 10000 || temp2.number_.size() > 10000) {
    throw BigIntegerOverflow();
  }

  if (this->sign_ != integer.sign_) {
    temp2.sign_ *= -1;
    return (*this) - temp2;
  }

  if (temp1.number_.size() < integer.number_.size()) {
    std::swap(temp1.number_, temp2.number_);
  }

  size_t max_size = temp1.number_.size();
  std::vector<int> result(max_size, 0);

  for (size_t i = 0; i < max_size; ++i) {
    int num1 = (i < temp1.number_.size())
                   ? std::stoi(temp1.number_[temp1.number_.size() - 1 - i])
                   : 0;
    int num2 = (i < temp2.number_.size())
                   ? std::stoi(temp2.number_[temp2.number_.size() - 1 - i])
                   : 0;
    result[i] += num1 + num2;
    if (result[i] >= 100000 && i + 1 < max_size) {
      result[i] -= 100000;
      result[i + 1]++;
    } else if (result[i] >= 100000 && i + 1 == max_size) {
      result[i] -= 100000;
      result.push_back(1);
    }
  }

  temp1.number_ = AddZeros(result);
  BigInteger result_big_integer = ChangeSignToZeroNumber(temp1);

  if (result_big_integer.number_.size() > 10000) {
    throw BigIntegerOverflow();
  }

  return result_big_integer;
}

BigInteger& BigInteger::operator+=(const BigInteger& integer) {
  *this = *this + integer;
  return *this;
}

BigInteger BigInteger::operator-(const BigInteger& integer) const {
  BigInteger temp1 = *this;
  BigInteger temp2 = integer;

  if (this->sign_ != integer.sign_) {
    temp2.sign_ *= -1;
    return (*this) + temp2;
  }

  if (ComparisonForNegativeNumbers(temp1, temp2)) {
    std::swap(temp1.number_, temp2.number_);
    temp1.sign_ *= -1;
  }

  //  if (temp1 < temp2) {
  //    std::swap(temp1.number_, temp2.number_);
  //    temp1.sign_ *= -1;
  //  }

  //  if (flag && temp2.sign_ == -1) {
  //    std::swap(temp1.number_, temp2.number_);
  //    temp1.sign_ *= -1;
  //  }

  size_t max_size = temp1.number_.size();
  std::vector<int> result(max_size, 0);

  for (size_t i = 0; i < max_size; ++i) {
    int num1 = (i < temp1.number_.size())
                   ? std::stoi(temp1.number_[temp1.number_.size() - 1 - i])
                   : 0;
    int num2 = (i < temp2.number_.size())
                   ? std::stoi(temp2.number_[temp2.number_.size() - 1 - i])
                   : 0;
    result[i] = num1 - num2;
  }

  for (size_t i = 0; i < result.size(); ++i) {
    if (result[i] < 0 && result.size() != 1) {
      result[i] += 100000;
      result[i + 1]--;
    }
  }

  temp1.number_ = AddZeros(result);
  temp1.number_ = RemoveLeadingZeros(temp1.number_);
  BigInteger result_big_integer = ChangeSignToZeroNumber(temp1);
  return result_big_integer;
}

std::vector<std::string> BigInteger::RemoveLeadingZeros(
    const std::vector<std::string>& number) const {
  std::vector<std::string> result = number;

  while (!result.empty() &&
         result.front().find_first_not_of('0') == std::string::npos) {
    result.erase(result.begin());
  }

  if (result.empty()) {
    result.emplace_back("0");
  } else {
    result.front().erase(0, result.front().find_first_not_of('0'));
  }

  return result;
}

BigInteger& BigInteger::operator-=(const BigInteger& integer) {
  *this = *this - integer;
  return *this;
}

BigInteger BigInteger::operator*(const BigInteger& integer) const {
  BigInteger temp1 = *this;
  BigInteger temp2 = integer;

  if (temp1.number_.size() > 10000 || temp2.number_.size() > 10000) {
    throw BigIntegerOverflow();
  }

  size_t max_size = std::max(temp1.number_.size(), temp2.number_.size());
  std::vector<int64_t> result(max_size * 2, 0);

  std::reverse(temp1.number_.begin(), temp1.number_.end());
  std::reverse(temp2.number_.begin(), temp2.number_.end());

  for (size_t i = 0; i < temp1.number_.size(); ++i) {
    for (size_t j = 0; j < temp2.number_.size(); ++j) {
      result[i + j] +=
          std::stol(temp1.number_[i]) * std::stol(temp2.number_[j]);
    }
  }

  for (size_t i = 0; i < result.size() - 1; ++i) {
    result[i + 1] += result[i] / 100000;
    result[i] %= 100000;
  }

  while (result.size() > 1 && result.back() == 0) {
    result.pop_back();
  }

  std::vector<int> int_result(result.size(), 0);
  for (size_t i = 0; i < result.size(); ++i) {
    int_result[i] = static_cast<int>(result[i]);
  }

  temp1.number_ = AddZeros(int_result);
  BigInteger result_big_integer = ChangeSignToZeroNumber(temp1);

  if (temp1.sign_ != temp2.sign_) {
    result_big_integer.sign_ = -1;
  } else {
    result_big_integer.sign_ = 1;
  }

  if (result_big_integer.number_.size() > 10000) {
    throw BigIntegerOverflow();
  }

  return result_big_integer;
}

BigInteger& BigInteger::operator*=(const BigInteger& integer) {
  *this = *this * integer;
  return *this;
}

BigInteger& BigInteger::operator++() {
  *this = *this + BigInteger(1);
  return *this;
}

const BigInteger BigInteger::operator++(int) {
  BigInteger before = *this;
  ++(*this);
  return before;
}

BigInteger& BigInteger::operator--() {
  *this = *this - BigInteger(1);
  return *this;
}
const BigInteger BigInteger::operator--(int) {
  BigInteger before = *this;
  --(*this);
  return before;
}

bool BigInteger::operator==(const BigInteger& integer) const {
  return (this->number_ == integer.number_) && (this->sign_ == integer.sign_);
}

bool BigInteger::operator!=(const BigInteger& integer) const {
  return !(*this == integer);
}

BigInteger::operator bool() const {
  if (number_.empty()) {
    return false;
  }

  for (const auto& digit : number_) {
    if (digit != "0") {
      return true;
    }
  }
  return false;
}

std::ostream& operator<<(std::ostream& ostream, const BigInteger& integer) {
  bool first_zeros = false;
  if (!integer.number_.empty()) {
    first_zeros = (std::stoi(integer.number_[0]) == 0);
  }

  std::string result_string;
  for (const auto& num : integer.number_) {
    result_string += num;
  }

  if (integer.sign_ == -1) {
    ostream << "-";
  }

  for (size_t i = 0; i < result_string.size(); ++i) {
    if (result_string[i] == '0' && first_zeros &&
        i != result_string.size() - 1) {
      continue;
    }

    first_zeros = false;
    ostream << result_string[i];
  }

  return ostream;
}

std::istream& operator>>(std::istream& istream, BigInteger& integer) {
  std::string str;
  istream >> str;

  if (str.length() > str.max_size()) {
    throw BigIntegerOverflow();
  }

  std::string char_to_string_with_sign = std::string(str);
  std::string sub_str = str.substr(1);

  if (str[0] == '-') {
    integer.sign_ *= -1;
    integer.number_ = integer.ParseStringToVector(sub_str, 5);
  } else if (str[0] == '+') {
    integer.number_ = integer.ParseStringToVector(sub_str, 5);
  } else {
    integer.number_ = integer.ParseStringToVector(str, 5);
  }

  return istream;
}
