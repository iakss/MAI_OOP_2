#pragma once

#include <initializer_list>
#include <string>

namespace lib::array {

const std::size_t DEFAULT_CAPACITY = 10;

class Array {
public:
  Array();
  Array(const std::size_t size, const unsigned char value = 0);
  Array(const std::initializer_list<unsigned char> &init);
  Array(const std::string &init);

  Array(const Array &other);
  Array(Array &&other) noexcept;
  ~Array() noexcept;

  unsigned char Get(const std::size_t index) const noexcept;
  unsigned char Front() const noexcept;
  unsigned char Back() const noexcept;
  unsigned char *Data() const noexcept;

  bool Empty() const noexcept;
  std::size_t Size() const noexcept;
  void Reserve(const std::size_t new_cap);
  std::size_t Capacity() const noexcept;

  void Clear() noexcept;
  void PushBack(const unsigned char value);
  void PopBack() noexcept;

private:
  std::size_t capacity_ = 0;
  std::size_t size_ = 0;
  unsigned char *data_ = nullptr;
};

} // namespace lib::array
