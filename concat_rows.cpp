#include <cstddef>
#include <iostream>
#include <limits>
#include <new>
#include <stdexcept>

namespace tsulyauf {
  void deleteMatrix(int **matrix, const std::size_t rows)
  {
    if (matrix != nullptr) {
      for (std::size_t i = 0; i < rows; ++i) {
        delete[] matrix[i];
      }
      delete[] matrix;
    }
  }

  int **getMatrix(std::size_t rows, std::size_t columns)
  {
    int **matrix = new int *[rows] {};

    try {
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
      deleteMatrix(matrix, rows);
      throw;
    }

    return matrix;
  }

  int **concat_rows(const int *const *lhs, const std::size_t n1, const std::size_t m1, const int *const *rhs,
                    const std::size_t n2, const std::size_t m2)
  {
    std::size_t max_n = n1 > n2 ? n1 : n2;

    if (m1 > std::numeric_limits<std::size_t>::max() - m2) {
      throw std::overflow_error("Переполнение размера массива");
    }

    int **matrix = new int *[max_n] {};

    try {
      for (std::size_t i = 0; i < max_n; ++i) {

        bool has_l = i < n1, has_r = i < n2;
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
      deleteMatrix(matrix, max_n);
      throw;
    }

    return matrix;
  }

  void printMatrix(const int *const *matrix, const std::size_t rows, const std::size_t columns)
  {
    for (std::size_t i = 0; i < rows; ++i) {
      for (std::size_t j = 0; j < columns; ++j) {
        std::cout << matrix[i][j] << " ";
      }
      std::cout << std::endl;
    }
  }

  void printWavyMatrix(const int *const *matrix, const std::size_t n1, const std::size_t m1, const std::size_t n2,
                       const std::size_t m2)
  {
    std::size_t max_n = 0, min_n = 0, max_n_m = 0;
    if (n1 > n2) {
      max_n = n1;
      min_n = n2;
      max_n_m = m1;
    } else {
      max_n = n2;
      min_n = n1;
      max_n_m = m2;
    }

    for (std::size_t i = 0; i < max_n; ++i) {
      if (i < min_n) {
        for (std::size_t j = 0; j < (m1 + m2); ++j) {
          std::cout << matrix[i][j];
          if (j < (m1 + m2 - 1)) {
            std::cout << " ";
          }
        }
      } else {
        for (std::size_t j = 0; j < max_n_m; ++j) {
          std::cout << matrix[i][j];
          if (j < (max_n_m - 1)) {
            std::cout << " ";
          }
        }
      }
      std::cout << "\n";
    }
  }
} // namespace tsulyauf

int main()
{
  const int err_invalid_argument = 1, err_bad_alloc = 2, err_overflow_error = 3;
  const int min_dimension = 1;
  long long rows1_in = 0, columns1_in = 0, rows2_in = 0, columns2_in = 0;

  std::cout << "Введите размер 1 матрицы: ";
  std::cin >> rows1_in >> columns1_in;
  std::cout << "\n";

  if (std::cin.fail() || rows1_in < min_dimension || columns1_in < min_dimension) {
    std::cerr << "Неправильный ввод размера 1 матрицы\n";
    return err_invalid_argument;
  }

  std::size_t rows1 = static_cast<std::size_t>(rows1_in);
  std::size_t columns1 = static_cast<std::size_t>(columns1_in);

  std::cout << "Введите размер 2 матрицы: ";
  std::cin >> rows2_in >> columns2_in;
  std::cout << "\n";

  if (std::cin.fail() || rows2_in < min_dimension || columns2_in < min_dimension) {
    std::cerr << "Неправильный ввод размера 2 матрицы\n";
    return err_invalid_argument;
  }

  std::size_t rows2 = static_cast<std::size_t>(rows2_in);
  std::size_t columns2 = static_cast<std::size_t>(columns2_in);

  int **matrix1 = nullptr;
  int **matrix2 = nullptr;

  try {
    std::cout << "Введите элементы 1 матрицы (" << rows1 << " строк, " << columns1 << " столбцов):\n";
    matrix1 = tsulyauf::getMatrix(rows1, columns1);
  } catch (const std::bad_alloc &) {
    std::cerr << "Ошибка выделения памяти для матрицы 1\n";
    return err_bad_alloc;
  } catch (const std::invalid_argument &e) {
    std::cerr << "Ошибка ввода: " << e.what() << "\n";
    return err_invalid_argument;
  }
  try {
    std::cout << "Введите элементы 2 матрицы (" << rows2 << " строк, " << columns2 << " столбцов): \n";
    matrix2 = tsulyauf::getMatrix(rows2, columns2);
  } catch (const std::bad_alloc &) {
    std::cerr << "Ошибка выделения памяти для матрицы 2\n";
    tsulyauf::deleteMatrix(matrix1, rows1);
    return err_bad_alloc;
  } catch (const std::invalid_argument &e) {
    std::cerr << "Ошибка ввода: " << e.what() << "\n";
    tsulyauf::deleteMatrix(matrix1, rows1);
    return err_invalid_argument;
  }

  int **matrix3 = nullptr;

  try {
    matrix3 = tsulyauf::concat_rows(matrix1, rows1, columns1, matrix2, rows2, columns2);
  } catch (const std::overflow_error &e) {
    std::cerr << "Ошибка переполнения: " << e.what() << "\n";
    tsulyauf::deleteMatrix(matrix1, rows1);
    tsulyauf::deleteMatrix(matrix2, rows2);
    return err_overflow_error;
  } catch (const std::bad_alloc &) {
    std::cerr << "Ошибка выделения памяти для матрицы 3\n";
    tsulyauf::deleteMatrix(matrix1, rows1);
    tsulyauf::deleteMatrix(matrix2, rows2);
    return err_bad_alloc;
  }

  std::cout << "Матрица 1:\n";
  tsulyauf::printMatrix(matrix1, rows1, columns1);
  std::cout << "Матрица 2:\n";
  tsulyauf::printMatrix(matrix2, rows2, columns2);
  std::cout << "Матрица 3:\n";
  tsulyauf::printWavyMatrix(matrix3, rows1, columns1, rows2, columns2);
  tsulyauf::deleteMatrix(matrix1, rows1);
  tsulyauf::deleteMatrix(matrix2, rows2);
  tsulyauf::deleteMatrix(matrix3, rows1 > rows2 ? rows1 : rows2);
}
