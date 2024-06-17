#define MATRIX_SQUARE_MATRIX_IMPLEMENTED
#ifndef MATRIX_H
#define MATRIX_H

#include <array>
#include <iostream>
#include <type_traits>

class MatrixOutOfRange : std::out_of_range {
 public:
  MatrixOutOfRange() : std::out_of_range("MatrixOutOfRange") {}
};

class MatrixIsDegenerateError : std::runtime_error {
 public:
  MatrixIsDegenerateError() : std::runtime_error("MatrixIsDegenerateError") {}
};

template <class T, size_t N, size_t M>
class Matrix {
 public:
  T data[N][M];

  size_t ColumnsNumber() const { return M; }

  size_t RowsNumber() const { return N; }

  const T& operator()(size_t row, size_t col) const { return data[row][col]; };

  T& operator()(size_t row, size_t col) { return data[row][col]; }

  T& At(size_t row, size_t col) {
    if (row >= N || col >= M) {
      throw MatrixOutOfRange();
    }

    return data[row][col];
  }

  const T& At(size_t row, size_t col) const {
    if (row >= N || col >= M) {
      throw MatrixOutOfRange();
    }

    return data[row][col];
  }

  Matrix<T, N, M> operator+(const Matrix<T, N, M>& matrix) const {
    Matrix<T, N, M> new_matrix;
    for (size_t i = 0; i < N; ++i) {
      for (size_t j = 0; j < M; ++j) {
        new_matrix(i, j) = data[i][j] + matrix.data[i][j];
      }
    }

    return new_matrix;
  }

  Matrix<T, N, M> operator-(const Matrix<T, N, M>& matrix) const {
    Matrix<T, N, M> new_matrix;
    for (size_t i = 0; i < N; ++i) {
      for (size_t j = 0; j < M; ++j) {
        new_matrix(i, j) = data[i][j] - matrix.data[i][j];
      }
    }

    return new_matrix;
  }

  template <size_t K>
  Matrix<T, N, K> operator*(const Matrix<T, M, K>& matrix) const {
    Matrix<T, N, K> new_matrix;
    for (size_t i = 0; i < N; ++i) {
      for (size_t j = 0; j < K; ++j) {
        new_matrix(i, j) = 0;
        for (size_t k = 0; k < M; ++k) {
          new_matrix(i, j) += data[i][k] * matrix.data[k][j];
        }
      }
    }

    return new_matrix;
  }

  Matrix<T, N, M>& operator+=(const Matrix<T, N, M>& matrix) {
    for (size_t i = 0; i < N; ++i) {
      for (size_t j = 0; j < M; ++j) {
        data[i][j] += matrix.data[i][j];
      }
    }
    return *this;
  }

  Matrix<T, N, M>& operator-=(const Matrix<T, N, M>& matrix) {
    for (size_t i = 0; i < N; ++i) {
      for (size_t j = 0; j < M; ++j) {
        data[i][j] -= matrix.data[i][j];
      }
    }
    return *this;
  }

  Matrix<T, N, M>& operator*=(const Matrix<T, M, M>& matrix) {
    Matrix<T, N, M> result = *this * matrix;
    *this = result;
    return *this;
  }

  Matrix<T, N, M> operator*(const T number) const {
    Matrix<T, N, M> new_matrix;
    for (size_t i = 0; i < N; ++i) {
      for (size_t j = 0; j < M; ++j) {
        new_matrix(i, j) = data[i][j] * number;
      }
    }

    return new_matrix;
  }

  friend Matrix<T, N, M> operator*(const T number,
                                   const Matrix<T, N, M>& matrix) {
    return matrix * number;
  }

  Matrix<T, N, M> operator/(const T number) const {
    Matrix<T, N, M> new_matrix;
    for (size_t i = 0; i < N; ++i) {
      for (size_t j = 0; j < M; ++j) {
        new_matrix(i, j) = data[i][j] / number;
      }
    }

    return new_matrix;
  }

  friend Matrix<T, N, M> operator/(const T number,
                                   const Matrix<T, N, M>& matrix) {
    return matrix / number;
  }

  Matrix<T, N, M>& operator*=(const T number) {
    for (size_t i = 0; i < N; ++i) {
      for (size_t j = 0; j < M; ++j) {
        data[i][j] *= number;
      }
    }

    return *this;
  }

  Matrix<T, N, M>& operator/=(const T number) {
    for (size_t i = 0; i < N; ++i) {
      for (size_t j = 0; j < M; ++j) {
        data[i][j] /= number;
      }
    }

    return *this;
  }

  friend std::ostream& operator<<(std::ostream& stream,
                                  const Matrix<T, N, M>& matrix) {
    for (size_t i = 0; i < N; ++i) {
      for (size_t j = 0; j < M; ++j) {
        stream << matrix.data[i][j];
        if (j != M - 1) {
          stream << " ";
        }
      }

      stream << "\n";
    }

    return stream;
  }

  friend std::istream& operator>>(std::istream& stream,
                                  Matrix<T, N, M>& matrix) {
    for (size_t i = 0; i < N; ++i) {
      for (size_t j = 0; j < M; ++j) {
        stream >> matrix(i, j);
      }
    }
    return stream;
  }
};

template <class T, size_t N, size_t M>
bool operator==(const Matrix<T, N, M> matrix1, const Matrix<T, N, M> matrix2) {
  for (size_t i = 0; i < N; ++i) {
    for (size_t j = 0; j < M; ++j) {
      if (matrix1.data[i][j] != matrix2.data[i][j]) {
        return false;
      }
    }
  }

  return true;
}

template <class T, size_t N, size_t M>
bool operator!=(const Matrix<T, N, M> matrix1, const Matrix<T, N, M> matrix2) {
  return !(matrix1 == matrix2);
}

template <class T, size_t N, size_t M>
Matrix<T, M, N> GetTransposed(const Matrix<T, N, M>& matrix) {
  Matrix<T, M, N> new_matrix;
  for (size_t i = 0; i < M; ++i) {
    for (size_t j = 0; j < N; ++j) {
      new_matrix(i, j) = matrix.data[j][i];
    }
  }

  return new_matrix;
}

template <class T, size_t N, size_t M>
void Transpose(Matrix<T, N, M>& matrix) {
  for (size_t i = 0; i < M; ++i) {
    for (size_t j = i + 1; j < N; ++j) {
      std::swap(matrix.data[i][j], matrix.data[j][i]);
    }
  }
}

template <class T, size_t N, size_t M>
T Trace(Matrix<T, N, M>& matrix) {
  T trace = 0;
  for (size_t i = 0; i < N; ++i) {
    trace += matrix.data[i][i];
  }

  return trace;
}

template <class T, size_t N, size_t M>
T Determinant(const Matrix<T, N, M>& matrix) {
  Matrix<T, N, M> new_matrix = matrix;
  for (size_t k = 0; k < N - 1; ++k) {
    for (size_t i = k + 1; i < N; ++i) {
      for (size_t j = k + 1; j < N; ++j) {
        new_matrix.data[i][j] = new_matrix.data[k][k] * new_matrix.data[i][j] -
                                new_matrix.data[i][k] * new_matrix.data[k][j];
        if (k != 0) {
          new_matrix.data[i][j] /= new_matrix.data[k - 1][k - 1];
        }
      }
    }
  }

  return new_matrix.data[N - 1][N - 1];
}

template <class T, size_t N>
Matrix<T, N - 1, N - 1> AlgebraicComplement(const Matrix<T, N, N>& matrix,
                                            size_t i, size_t j) {
  Matrix<T, N - 1, N - 1> new_matrix;
  size_t sub_i = 0;

  for (size_t row = 0; row < N; ++row) {
    if (row == i) {
      continue;
    }
    size_t sub_j = 0;
    for (size_t col = 0; col < N; ++col) {
      if (col == j) {
        continue;
      }
      new_matrix.data[sub_i][sub_j] = matrix.data[row][col];
      ++sub_j;
    }
    ++sub_i;
  }

  return new_matrix;
}

template <class T, size_t N>
Matrix<T, N, N> GetInversed(const Matrix<T, N, N>& matrix) {
  if (Determinant(matrix) == 0) {
    throw MatrixIsDegenerateError();
  }

  Matrix<T, N, N> new_matrix;
  for (size_t i = 0; i < N; ++i) {
    for (size_t j = 0; j < N; ++j) {
      Matrix<T, N - 1, N - 1> minor = AlgebraicComplement(matrix, i, j);
      new_matrix(j, i) =
          (i + j) % 2 == 0 ? Determinant(minor) : -Determinant(minor);
    }
  }

  return (new_matrix / Determinant(matrix));
}

template <class T, size_t N>
void Inverse(Matrix<T, N, N>& matrix) {
  matrix = GetInversed(matrix);
}

template <class T>
Matrix<T, 1, 1> GetInversed(const Matrix<T, 1, 1>& matrix) {
  if (Determinant(matrix) == 0) {
    throw MatrixIsDegenerateError();
  }
  Matrix<T, 1, 1> new_matrix;
  new_matrix.data[0][0] = T{1} / matrix.data[0][0];
  return new_matrix;
}

template <class T>
void Inverse(Matrix<T, 1, 1>& matrix) {
  matrix = GetInversed(matrix);
}

#endif
