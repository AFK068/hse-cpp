#ifndef VECTOR_H
#define VECTOR_H

#include <algorithm>
#include <exception>
#include <iostream>
#include <iterator>
#include <type_traits>

template <class T>
class Vector {
 public:
  using ValueType = T;
  using Pointer = T*;
  using ConstPointer = const T*;
  using Reference = T&;
  using ConstReference = const T&;
  using SizeType = std::size_t;
  using Iterator = T*;
  using ConstIterator = const T*;
  using ReverseIterator = std::reverse_iterator<Iterator>;
  using ConstReverseIterator = std::reverse_iterator<ConstIterator>;

  Vector() = default;
  explicit Vector(size_t count);
  Vector(size_t count, const T& value);
  Vector(const Vector& other);
  Vector(Vector&& other) noexcept;
  Vector(std::initializer_list<T> list);
  template <class Iterator,
            class = std::enable_if_t<std::is_base_of_v<
                std::forward_iterator_tag,
                typename std::iterator_traits<Iterator>::iterator_category>>>
  Vector(Iterator first, Iterator last) {
    try {
      for (auto it = first; it != last; ++it) {
        PushBack(*it);
      }
    } catch (...) {
      delete[] data_;
      data_ = nullptr;
      size_ = 0;
      capacity_ = 0;
      throw;
    }
  }

  size_t Size() const;
  size_t Capacity() const;
  bool Empty() const;

  Vector<T>& operator=(const Vector<T>&);
  Vector<T>& operator=(Vector&& other) noexcept;

  T& operator[](size_t index);
  const T& operator[](size_t index) const;

  T& At(size_t index);
  const T& At(size_t index) const;

  T& Front();
  const T& Front() const;
  T& Back();
  const T& Back() const;

  T* Data();
  const T* Data() const;

  void Swap(Vector&);

  void Resize(size_t);
  void Resize(size_t, const T&);
  void Reserve(size_t);
  void ShrinkToFit();
  void Clear();

  void PushBack(const T&);
  void PushBack(T&&);
  void PopBack();

  bool operator==(const Vector<T>& object) const;
  bool operator!=(const Vector<T>& object) const;
  bool operator<(const Vector<T>& object) const;
  bool operator<=(const Vector<T>& object) const;
  bool operator>(const Vector<T>& object) const;
  bool operator>=(const Vector<T>& object1) const;

  // Iterators
  Iterator begin() { return data_; };            // NOLINT
  ConstPointer begin() const { return data_; };  // NOLINT

  Iterator end() { return data_ + size_; };             // NOLINT
  ConstIterator end() const { return data_ + size_; };  // NOLINT

  ConstIterator cbegin() { return data_; };        // NOLINT
  ConstIterator cend() { return data_ + size_; };  // NOLINT

  ReverseIterator rbegin() {  // NOLINT
    return std::reverse_iterator<Pointer>(end());
  };
  ConstReverseIterator rbegin() const {  // NOLINT
    return std::reverse_iterator<ConstIterator>(end());
  };

  ReverseIterator rend() {  // NOLINT
    return std::reverse_iterator<Pointer>(begin());
  };
  ConstReverseIterator rend() const {  // NOLINT
    return std::reverse_iterator<ConstIterator>(begin());
  };

  ConstReverseIterator crbegin() {  // NOLINT
    return std::reverse_iterator<ConstIterator>(cend());
  };
  ConstReverseIterator crend() {  // NOLINT
    return std::reverse_iterator<ConstIterator>(cbegin());
  };

  ~Vector() { delete[] data_; }

 private:
  SizeType size_{};
  SizeType capacity_{};
  Pointer data_{};
};

template <class T>
Vector<T>::Vector(size_t count) : size_(count), capacity_(count) {
  try {
    data_ = (count > 0 ? new T[count] : nullptr);
  } catch (...) {
    delete[] data_;
    data_ = nullptr;
    size_ = 0;
    capacity_ = 0;
    throw;
  }
}

template <class T>
Vector<T>::Vector(size_t count, const T& value)
    : size_(count), capacity_(count) {
  try {
    data_ = (count > 0 ? new T[count] : nullptr);
  } catch (...) {
    delete[] data_;
    data_ = nullptr;
    size_ = 0;
    capacity_ = 0;
    throw;
  }
  try {
    std::fill(data_, data_ + count, value);
  } catch (...) {
    delete[] data_;
    data_ = nullptr;
    size_ = 0;
    capacity_ = 0;
    throw;
  }
}

template <class T>
Vector<T>::Vector(const Vector& other)
    : size_(other.size_), capacity_(other.capacity_) {
  try {
    data_ = (other.capacity_ > 0 ? new T[other.capacity_] : nullptr);
  } catch (...) {
    delete[] data_;
    data_ = nullptr;
    size_ = 0;
    capacity_ = 0;
    throw;
  }

  try {
    std::copy(other.data_, other.data_ + other.size_, data_);
  } catch (...) {
    delete[] data_;
    data_ = nullptr;
    size_ = 0;
    capacity_ = 0;
    throw;
  }
}

template <class T>
Vector<T>::Vector(Vector&& other) noexcept
    : size_(other.size_), capacity_(other.capacity_), data_(other.data_) {
  other.data_ = nullptr;
  other.capacity_ = 0;
  other.size_ = 0;
}

template <class T>
Vector<T>::Vector(std::initializer_list<T> list)
    : size_(list.size()), capacity_(list.size()) {
  try {
    data_ = (list.size() > 0 ? new T[list.size()] : nullptr);
  } catch (...) {
    delete[] data_;
    data_ = nullptr;
    size_ = 0;
    capacity_ = 0;
    throw;
  }

  try {
    std::copy(list.begin(), list.end(), data_);
  } catch (...) {
    delete[] data_;
    data_ = nullptr;
    size_ = 0;
    capacity_ = 0;
    throw;
  }
}

template <class T>
size_t Vector<T>::Size() const {
  return size_;
}

template <class T>
size_t Vector<T>::Capacity() const {
  return capacity_;
}

template <class T>
bool Vector<T>::Empty() const {
  return size_ == 0;
}

template <class T>
Vector<T>& Vector<T>::operator=(const Vector<T>& other) {
  if (this != &other) {
    Vector<T> temp(other);
    std::swap(*this, temp);
  }

  return *this;
}

template <class T>
Vector<T>& Vector<T>::operator=(Vector&& other) noexcept {
  if (this != &other) {
    delete[] data_;
    data_ = other.data_;
    size_ = other.size_;
    capacity_ = other.capacity_;
    other.data_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
  }

  return *this;
}

template <class T>
T& Vector<T>::operator[](size_t index) {
  return data_[index];
}

template <class T>
const T& Vector<T>::operator[](size_t index) const {
  return data_[index];
}

template <class T>
T& Vector<T>::At(size_t index) {
  if (index >= size_) {
    throw std::out_of_range("Out of range");
  }

  return data_[index];
}

template <class T>
const T& Vector<T>::At(size_t index) const {
  if (index >= size_) {
    throw std::out_of_range("Out of range");
  }

  return data_[index];
}

template <class T>
T& Vector<T>::Front() {
  return data_[0];
}

template <class T>
const T& Vector<T>::Front() const {
  return data_[0];
}

template <class T>
T& Vector<T>::Back() {
  return data_[size_ - 1];
}

template <class T>
const T& Vector<T>::Back() const {
  return data_[size_ - 1];
}

template <class T>
T* Vector<T>::Data() {
  return data_;
}

template <class T>
const T* Vector<T>::Data() const {
  return data_;
}

template <class T>
void Vector<T>::Swap(Vector& other) {
  std::swap(data_, other.data_);
  std::swap(size_, other.size_);
  std::swap(capacity_, other.capacity_);
}

template <class T>
void Vector<T>::Resize(size_t new_size) {
  if (new_size <= capacity_) {
    size_ = new_size;
  } else {
    T* new_ptr{};
    try {
      new_ptr = new T[new_size * 2];
      std::copy(std::make_move_iterator(data_),
                std::make_move_iterator(data_ + size_), new_ptr);
      size_ = new_size;
      delete[] data_;
      data_ = new_ptr;
      capacity_ = new_size * 2;
    } catch (...) {
      delete[] new_ptr;
      throw;
    }
  }
}

template <class T>
void Vector<T>::Resize(size_t new_size, const T& value) {
  if (new_size <= size_) {
    size_ = new_size;
  } else {
    if (new_size <= capacity_) {
      std::fill(data_ + size_, data_ + new_size, value);
      size_ = new_size;
    } else {
      T* new_ptr{};
      try {
        new_ptr = new T[new_size * 2];
        std::copy(std::make_move_iterator(data_),
                  std::make_move_iterator(data_ + size_), new_ptr);
        std::fill(new_ptr + size_, new_ptr + new_size, value);
        size_ = new_size;
        delete[] data_;
        data_ = new_ptr;
        capacity_ = new_size * 2;
      } catch (...) {
        delete[] new_ptr;
        throw;
      }
    }
  }
}

template <class T>
void Vector<T>::Reserve(size_t new_cap) {
  if (capacity_ >= new_cap) {
    return;
  }

  T* new_ptr{};
  try {
    new_ptr = new T[new_cap];
    std::copy(std::make_move_iterator(data_),
              std::make_move_iterator(data_ + size_), new_ptr);
    delete[] data_;
    capacity_ = new_cap;
    data_ = new_ptr;
  } catch (...) {
    delete[] new_ptr;
    throw;
  }
}

template <class T>
void Vector<T>::ShrinkToFit() {
  if (size_ == 0) {
    delete[] data_;
    data_ = nullptr;
    capacity_ = 0;
  } else if (capacity_ > size_) {
    T* new_ptr{};
    try {
      new_ptr = new T[size_];
      std::copy(std::make_move_iterator(data_),
                std::make_move_iterator(data_ + size_), new_ptr);
      delete[] data_;
      data_ = new_ptr;
      capacity_ = size_;
    } catch (...) {
      delete[] new_ptr;
      throw;
    }
  }
}

template <class T>
void Vector<T>::Clear() {
  size_ = 0;
}

template <class T>
void Vector<T>::PushBack(const T& value) {
  if (size_ == capacity_) {
    Reserve(size_ == 0 ? 1 : (size_ * 2) + 1);
  }

  data_[size_++] = value;
}

template <class T>
void Vector<T>::PushBack(T&& value) {
  if (size_ == capacity_) {
    Reserve(size_ == 0 ? 1 : (size_ * 2) + 1);
  }

  data_[size_++] = std::move(value);
}

template <class T>
void Vector<T>::PopBack() {
  if (size_ > 0) {
    --size_;
  }
}

template <class T>
bool Vector<T>::operator==(const Vector<T>& object) const {
  if (this->size_ != object.size_) {
    return false;
  }
  return std::equal(this->data_, this->data_ + this->size_, object.data_);
}

template <class T>
bool Vector<T>::operator!=(const Vector<T>& object) const {
  return !(*this == object);
}

template <class T>
bool Vector<T>::operator<(const Vector<T>& object) const {
  return std::lexicographical_compare(this->data_, this->data_ + this->size_,
                                      object.data_,
                                      object.data_ + object.size_);
}

template <class T>
bool Vector<T>::operator>(const Vector<T>& object) const {
  return object < *this;
}

template <class T>
bool Vector<T>::operator<=(const Vector<T>& object) const {
  return !(*this > object);
}

template <class T>
bool Vector<T>::operator>=(const Vector<T>& object) const {
  return !(*this < object);
}

#endif
