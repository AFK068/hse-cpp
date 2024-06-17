#define ARRAY_TRAITS_IMPLEMENTED
#ifndef ARRAY_H
#define ARRAY_H
#include <algorithm>

class ArrayOutOfRange : std::out_of_range {
 public:
  ArrayOutOfRange() : std::out_of_range("ArrayOutOfRange") {}
};

template <class T, size_t N>
class Array {
 public:
  T data[N];

  T& operator[](size_t number) { return data[number]; }

  const T& operator[](size_t number) const { return data[number]; }

  T& At(size_t number) {
    if (number >= N) {
      throw ArrayOutOfRange();
    }

    return data[number];
  }

  const T& At(size_t number) const {
    if (number >= N) {
      throw ArrayOutOfRange();
    }

    return data[number];
  }

  T& Back() { return data[N - 1]; }

  const T& Back() const { return data[N - 1]; }

  const T& Front() const { return data[0]; }
  T& Front() { return data[0]; }

  T* Data() { return data; }
  const T* Data() const { return data; }

  size_t Size() const { return N; }

  bool Empty() const { return N == 0; }

  void Fill(const T& value) { std::fill(data, data + N, value); }

  void Swap(Array<T, N>& other) {
    std::swap_ranges(data, data + N, other.data);
  }
};

template <class T>
size_t GetSize(T) {
  return 0;
}

template <class T, size_t N>
size_t GetSize(T (&)[N]) {
  return N;
}

template <class T>
size_t GetRank(T) {
  return 0;
}

template <class T, size_t N>
size_t GetRank(T (&array)[N]) {
  return 1 + GetRank(array[0]);
}

template <class T>
size_t GetNumElements(T) {
  return 1;
}

template <class T, size_t N>
size_t GetNumElements(T (&array)[N]) {
  return N * GetNumElements(array[0]);
}

#endif
