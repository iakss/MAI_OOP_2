#pragma once

#include "array.hpp"

#include <initializer_list>
#include <string>

namespace lib::bit_string {

class BitString {
public:
  BitString();
  BitString(const std::size_t count, const unsigned char value = 0);
  BitString(const std::initializer_list<unsigned char> &init);
  BitString(const std::string &init);

  BitString(array::Array &&array) noexcept;
  ~BitString() noexcept;

  bool Greater(const BitString &other) const noexcept;
  bool Smaller(const BitString &other) const noexcept;
  bool Equals(const BitString &other) const noexcept;

  static BitString And(const BitString &a, const BitString &b) noexcept;
  static BitString Or(const BitString &a, const BitString &b) noexcept;
  static BitString Xor(const BitString &a, const BitString &b) noexcept;
  static BitString Not(const BitString &str) noexcept;

private:
  const array::Array digits_;
};

} // namespace lib::bit_string
