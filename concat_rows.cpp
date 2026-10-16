#include <cstddef>
#include <iostream>
#include <limits>
#include <new>
#include <stdexcept>

namespace tsulyauf {
  void delete_matrix(int** matrix, const std::size_t rows)
  {
    if (matrix != nullptr) {
      for (std::size_t i = 0; i < rows; ++i) {
        delete[] matrix[i];
      }
      delete[] matrix;
    }
  }

  bool read_size(std::size_t& rows, std::size_t& columns)
  {
    long long rows_in = 0, columns_in = 0;
    std::cout << "Введите размеры матрицы: ";
    std::cin >> rows_in >> columns_in;
    std::cout << "\n";

    if (std::cin.fail() || rows_in < 0 || columns_in < 0) {
      return false;
    }

    rows = static_cast< std::size_t >(rows_in);
    columns = static_cast< std::size_t >(columns_in);

    return true;
  }

  int** get_matrix(const std::size_t rows, const std::size_t columns)
  {
    int** matrix = new int*[rows]{};

    try {
      std::cout << "Введите элементы матрицы:\n";

      for (std::size_t i = 0; i < rows; ++i) {
        matrix[i] = new int[columns];

        for (std::size_t j = 0; j < columns; ++j) {
          std::cin >> matrix[i][j];

          if (std::cin.fail()) {
            throw std::invalid_argument("Неправильный ввод элементов матрицы");
          }
        }
      }
    } catch (...) {
      delete_matrix(matrix, rows);
      throw;
    }

    return matrix;
  }

  int** concat_rows(const int* const* lhs, const std::size_t n1, const std::size_t m1, const int* const* rhs,
                    const std::size_t n2, const std::size_t m2)
  {
    if (m1 > std::numeric_limits< std::size_t >::max() - m2) {
      throw std::overflow_error("Переполнение размера массива");
    }

    const std::size_t max_n = n1 > n2 ? n1 : n2;
    int** matrix = new int*[max_n]{};

    try {
      for (std::size_t i = 0; i < max_n; ++i) {
        const bool has_l = i < n1;
        const bool has_r = i < n2;
        std::size_t len = 0;

        if (has_l && has_r) {
          len = m1 + m2;
        } else if (has_l) {
          len = m1;
        } else {
          len = m2;
        }

        matrix[i] = new int[len];

        for (std::size_t j = 0; j < len; ++j) {
          if (has_l && j < m1) {
            matrix[i][j] = lhs[i][j];
          } else if (has_l && has_r && (j - m1) < m2) {
            matrix[i][j] = rhs[i][j - m1];
          } else if (has_r && j < m2) {
            matrix[i][j] = rhs[i][j];
          }
        }
      }
    } catch (...) {
      delete_matrix(matrix, max_n);
      throw;
    }

    return matrix;
  }

  void print_row(const int* row, const std::size_t length)
  {
    for (std::size_t j = 0; j < length; ++j) {
      if (j > 0) {
        std::cout << " ";
      }
      std::cout << row[j];
    }
    std::cout << "\n";
  }

  void print_matrix(const int* const* matrix, const std::size_t rows, const std::size_t columns)
  {
    for (std::size_t i = 0; i < rows; ++i) {
      print_row(matrix[i], columns);
    }
  }

  void print_wavy_matrix(const int* const* matrix, const std::size_t n1, const std::size_t m1, const std::size_t n2,
                         const std::size_t m2)
  {
    std::size_t max_n = 0, min_n = 0, tail_len = 0;
    if (n1 > n2) {
      max_n = n1;
      min_n = n2;
      tail_len = m1;
    } else {
      max_n = n2;
      min_n = n1;
      tail_len = m2;
    }

    for (std::size_t i = 0; i < max_n; ++i) {
      if (i < min_n) {
        print_row(matrix[i], m1 + m2);
      } else {
        print_row(matrix[i], tail_len);
      }
    }
  }
} // namespace tsulyauf

int main()
{
  const int err_invalid_argument = 1, err_internal = 2;

  std::size_t rows1 = 0, columns1 = 0, rows2 = 0, columns2 = 0;

  if (!tsulyauf::read_size(rows1, columns1) || !tsulyauf::read_size(rows2, columns2)) {
    std::cerr << "Неправильный ввод размеров матриц\n";
    return err_invalid_argument;
  }

  int** matrix1 = nullptr;
  int** matrix2 = nullptr;
  int** matrix3 = nullptr;
  int status = 0;

  try {
    matrix1 = tsulyauf::get_matrix(rows1, columns1);
    matrix2 = tsulyauf::get_matrix(rows2, columns2);
    matrix3 = tsulyauf::concat_rows(matrix1, rows1, columns1, matrix2, rows2, columns2);

    std::cout << "Матрица 1:\n";
    tsulyauf::print_matrix(matrix1, rows1, columns1);
    std::cout << "Матрица 2:\n";
    tsulyauf::print_matrix(matrix2, rows2, columns2);
    std::cout << "Матрица 3:\n";
    tsulyauf::print_wavy_matrix(matrix3, rows1, columns1, rows2, columns2);

  } catch (const std::bad_alloc& e) {
    std::cerr << "Ошибка выделения памяти: " << e.what() << "\n";
    status = err_internal;
  } catch (const std::invalid_argument& e) {
    std::cerr << "Ошибка ввода: " << e.what() << "\n";
    status = err_invalid_argument;
  } catch (const std::overflow_error& e) {
    std::cerr << "Ошибка переполнения: " << e.what() << "\n";
    status = err_internal;
  }
  tsulyauf::delete_matrix(matrix1, rows1);
  tsulyauf::delete_matrix(matrix2, rows2);
  tsulyauf::delete_matrix(matrix3, rows1 > rows2 ? rows1 : rows2);

  return status;
}
