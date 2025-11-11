#include "array.hpp"

#include <cstring>
#include <stdexcept>

namespace lib::array {

Array::Array() { Reserve(DEFAULT_CAPACITY); }

Array::Array(const std::size_t count, const unsigned char value) {
  Reserve(count);
  size_ = count;
  for (std::size_t i = 0; i < size_; ++i) {
    data_[i] = value;
  }
}

Array::Array(const std::initializer_list<unsigned char> &init) {
  Reserve(init.size());
  size_ = init.size();
  size_t index = 0;
  for (unsigned char ch : init) {
    data_[index] = ch;
    ++index;
  }
}

Array::Array(const std::string &init) {
  Reserve(init.size());
  size_ = init.size();
  for (std::size_t i = 0; i < init.size(); ++i) {
    data_[i] = static_cast<unsigned char>(init[i]);
  }
}

Array::Array(const Array &other) {
  Reserve(other.capacity_);
  size_ = other.size_;
  for (std::size_t i = 0; i < other.size_; ++i) {
    data_[i] = other.data_[i];
  }
}

Array::Array(Array &&other) noexcept
    : capacity_(other.capacity_), size_(other.size_), data_(other.data_) {
  other.capacity_ = 0;
  other.size_ = 0;
  other.data_ = nullptr;
}

Array::~Array() noexcept { delete[] data_; }

unsigned char Array::Get(const std::size_t index) const noexcept {
  return data_[index];
}

unsigned char Array::Front() const noexcept { return data_[0]; }

unsigned char Array::Back() const noexcept { return data_[size_ - 1]; }

unsigned char *Array::Data() const noexcept { return data_; }

bool Array::Empty() const noexcept { return size_ == 0; }

std::size_t Array::Size() const noexcept { return size_; }

void Array::Reserve(const std::size_t new_cap) {
  if (capacity_ < new_cap or capacity_ == 0) {
    const std::size_t to_alloc_cap = std::max(new_cap, DEFAULT_CAPACITY);
    unsigned char *new_data = new unsigned char[to_alloc_cap];
    if (data_) {
      std::memcpy(new_data, data_, size_);
      delete[] data_;
    }
    capacity_ = to_alloc_cap;
    data_ = new_data;
  }
}

std::size_t Array::Capacity() const noexcept { return capacity_; }

void Array::Clear() noexcept {
  for (std::size_t i = 0; i < size_; ++i) {
    data_[i] = 0;
  }
  size_ = 0;
}

void Array::PushBack(const unsigned char value) {
  if (size_ == capacity_) {
    Reserve(size_ * 2);
  }
  data_[size_] = value;
  ++size_;
}

void Array::PopBack() noexcept {
  data_[size_ - 1] = 0;
  --size_;
}

} // namespace lib::array
