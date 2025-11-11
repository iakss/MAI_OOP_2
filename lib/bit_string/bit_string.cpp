#include "bit_string.hpp"

#include <stdexcept>

namespace {
inline bool is_bit(const unsigned char ch) noexcept {
  return ((ch == '0') or (ch == '1'));
}

bool has_only_bits(const std::initializer_list<unsigned char> &i) noexcept {
  if (i.size() == 0) {
    return true;
  }
  for (unsigned char ch : i) {
    if (!is_bit(ch)) {
      return false;
    }
  }
  return true;
}

bool has_only_bits(const std::string &str) noexcept {
  if (str.empty()) {
    return true;
  }
  for (unsigned char ch : str) {
    if (!is_bit(static_cast<unsigned char>(ch))) {
      return false;
    }
  }
  return true;
}

void RemoveLeadingZeroes(lib::array::Array &arr) noexcept {
  if (!arr.Empty()) {
    while (arr.Back() == '0' and arr.Size() != 1) {
      arr.PopBack();
    }
  }
}
} // namespace

namespace lib::bit_string {

BitString::BitString() {}

BitString::BitString(const std::size_t count, const unsigned char value)
    : digits_(count, value) {
  if (value >= '2') {
    throw std::invalid_argument("The value has to be equal '0' or '1'");
  }
  RemoveLeadingZeroes(digits_);
}

BitString::BitString(const std::initializer_list<unsigned char> &init)
    : digits_(init) {
  if (!has_only_bits(init)) {
    throw std::invalid_argument(
        "The argument must contain only ones and zeroes");
  };
  RemoveLeadingZeroes(digits_);
}

BitString::BitString(const std::string &init) : digits_(init) {
  if (!has_only_bits(init)) {
    throw std::invalid_argument(
        "The argument must contain only ones and zeroes");
  }
  RemoveLeadingZeroes(digits_);
}

BitString::BitString(const array::Array &arr) : digits_(arr) {
  for (std::size_t i = 0; i < digits_.Size(); ++i) {
    if (!is_bit(digits_.Get(i))) {
      throw std::invalid_argument("Array must contain only '0' and '1'!");
    }
  }
  RemoveLeadingZeroes(digits_);
}

BitString::BitString(array::Array &&arr) : digits_(std::move(arr)) {
  for (std::size_t i = 0; i < digits_.Size(); ++i) {
    if (!is_bit(digits_.Get(i))) {
      throw std::invalid_argument("Array must contain only '0' and '1'!");
    }
  }
  RemoveLeadingZeroes(digits_);
}

BitString::~BitString() noexcept = default;

BitString BitString::Add(const BitString &a, const BitString &b) noexcept {
  std::size_t max_size = std::max(a.digits_.Size(), b.digits_.Size());
  array::Array new_digits(max_size + 1, '0');
  unsigned char carry = '0';
  for (std::size_t i = 0; i < max_size; ++i) {
    unsigned char a_bit = (i < a.digits_.Size()) ? a.digits_.Get(i) : '0';
    unsigned char b_bit = (i < b.digits_.Size()) ? b.digits_.Get(i) : '0';
    int sum = static_cast<int>(a_bit - '0' + b_bit - '0' + carry - '0');
    new_digits.Data()[i] = static_cast<unsigned char>(sum % 2 + '0');
    carry = static_cast<unsigned char>(static_cast<int>(sum / 2) + '0');
  }
  if (carry == '1') {
    new_digits.Data()[max_size] = '1';
  } else {
    new_digits.PopBack();
  }
  RemoveLeadingZeroes(new_digits);
  return BitString(std::move(new_digits));
}

BitString BitString::Substract(const BitString &a, const BitString &b) {
  if (b.digits_.Empty()) {
    return BitString((a.digits_.Empty()) ? '0' : a.digits_);
  }
  if (a.Equals(b)) {
    return BitString{'0'};
  }
  if (a.Smaller(b)) {
    throw std::invalid_argument(
        "The first argument must be greater than the second one to substract!");
  }
  std::size_t max_size = a.digits_.Size();
  array::Array new_digits(max_size, '0');
  unsigned char carry = '0';
  for (std::size_t i = 0; i < max_size; ++i) {
    unsigned char a_bit = a.digits_.Get(i);
    unsigned char b_bit = (i < b.digits_.Size()) ? b.digits_.Get(i) : '0';
    int diff = static_cast<int>((a_bit - '0') - (b_bit - '0') - (carry - '0'));
    if (diff < 0) {
      diff += 2;
      carry = '1';
    } else {
      carry = '0';
    }
    new_digits.Data()[i] = static_cast<unsigned char>(diff + '0');
  }
  RemoveLeadingZeroes(new_digits);
  return BitString(std::move(new_digits));
}

bool BitString::Greater(const BitString &other) const noexcept {
  if (digits_.Size() != other.digits_.Size()) {
    return ((digits_.Size() > other.digits_.Size()) ? true : false);
  }
  std::size_t size = digits_.Size();
  for (std::size_t i = 1; i <= size; ++i) {
    if (digits_.Get(size - i) != other.digits_.Get(size - i)) {
      return ((digits_.Get(size - i) > other.digits_.Get(size - i)) ? true
                                                                    : false);
    }
  }
  return false;
}

bool BitString::Smaller(const BitString &other) const noexcept {
  return !Greater(other);
}

bool BitString::Equals(const BitString &other) const noexcept {
  if (digits_.Size() != other.digits_.Size()) {
    return false;
  }
  for (std::size_t i = 0; i < digits_.Size(); ++i) {
    if (digits_.Get(i) != other.digits_.Get(i)) {
      return false;
    }
  }
  return true;
}

BitString BitString::And(const BitString &a, const BitString &b) noexcept {
  std::size_t a_sz = a.digits_.Size();
  std::size_t b_sz = b.digits_.Size();
  std::size_t max_sz = std::max(a_sz, b_sz);
  array::Array new_digits(max_sz);
  for (std::size_t i = 0; i < max_sz; ++i) {
    unsigned char a_bit = (i < a_sz) ? a.digits_.Get(i) : '0';
    unsigned char b_bit = (i < b_sz) ? b.digits_.Get(i) : '0';
    new_digits.Data()[i] = (a_bit == '1' && b_bit == '1') ? '1' : '0';
  }
  RemoveLeadingZeroes(new_digits);
  return BitString(std::move(new_digits));
}

BitString BitString::Or(const BitString &a, const BitString &b) noexcept {
  std::size_t a_sz = a.digits_.Size();
  std::size_t b_sz = b.digits_.Size();
  std::size_t max_sz = std::max(a_sz, b_sz);
  array::Array new_digits(max_sz);
  for (std::size_t i = 0; i < max_sz; ++i) {
    unsigned char a_bit = (i < a_sz) ? a.digits_.Get(i) : '0';
    unsigned char b_bit = (i < b_sz) ? b.digits_.Get(i) : '0';
    new_digits.Data()[i] = (a_bit == '1' || b_bit == '1') ? '1' : '0';
  }
  return BitString(std::move(new_digits));
}

BitString BitString::Xor(const BitString &a, const BitString &b) noexcept {
  std::size_t a_sz = a.digits_.Size();
  std::size_t b_sz = b.digits_.Size();
  std::size_t max_sz = std::max(a_sz, b_sz);
  array::Array new_digits(max_sz);
  for (std::size_t i = 0; i < max_sz; ++i) {
    unsigned char a_bit = (i < a_sz) ? a.digits_.Get(i) : '0';
    unsigned char b_bit = (i < b_sz) ? b.digits_.Get(i) : '0';
    new_digits.Data()[i] = (a_bit == b_bit) ? '0' : '1';
  }
  RemoveLeadingZeroes(new_digits);
  return BitString(std::move(new_digits));
}

BitString BitString::Not(const BitString &str) noexcept {
  array::Array new_digits(str.digits_.Size());
  for (std::size_t i = 0; i < new_digits.Size(); ++i) {
    new_digits.Data()[i] = (str.digits_.Get(i) == '0') ? '1' : '0';
  }
  RemoveLeadingZeroes(new_digits);
  return BitString(std::move(new_digits));
}

} // namespace lib::bit_string
