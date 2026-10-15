#include <cstddef>
#include <iostream>
#include <new>

namespace tsulyauf {
  void printMatrix(const int *const *matrix, const std::size_t rows, const std::size_t columns)
  {
    for (std::size_t i = 0; i < rows; ++i) {
      for (std::size_t j = 0; j < columns; ++j) {
        std::cout << matrix[i][j] << " ";
      }
      std::cout << std::endl;
    }
  }

  void deleteMatrix(int **matrix, const std::size_t rows)
  {
    if (matrix == nullptr) {
      return;
    }
    for (std::size_t i = 0; i < rows; ++i) {
      delete[] matrix[i];
    }
    delete[] matrix;
  }
} // namespace tsulyauf

int main()
{
  const int invalid_input = 1;
  const int invalid_memory_usage = 2;

  long long rows_in = 0, columns_in = 0;
  std::cin >> rows_in >> columns_in;

  if (std::cin.fail() || rows_in <= 0 || columns_in <= 0) {
    std::cerr << "Invalid rows and columns input" << std::endl;
    return invalid_input;
  }

  const std::size_t rows = rows_in;
  const std::size_t columns = columns_in;

  int **matrix = nullptr;
  int **transposed_matrix = nullptr;

  try {
    matrix = new int *[rows]{};

    for (std::size_t i = 0; i < rows; ++i) {
      matrix[i] = new int[columns]{};

      for (std::size_t j = 0; j < columns; ++j) {
        std::cin >> matrix[i][j];

        if (std::cin.fail()) {
          std::cerr << "Invalid number input" << std::endl;
          tsulyauf::deleteMatrix(matrix, rows);
          return invalid_input;
        }
      }
    }

    transposed_matrix = new int *[columns]{};

    for (std::size_t i = 0; i < columns; ++i) {
      transposed_matrix[i] = new int[rows]{};
      for (std::size_t j = 0; j < rows; ++j) {
        transposed_matrix[i][j] = matrix[j][i];
      }
    }

    tsulyauf::printMatrix(transposed_matrix, columns, rows);
    tsulyauf::deleteMatrix(matrix, rows);
    tsulyauf::deleteMatrix(transposed_matrix, columns);

  } catch (const std::bad_alloc &e) {
    std::cerr << "Memory allocation failed: " << e.what() << std::endl;
    tsulyauf::deleteMatrix(matrix, rows);
    tsulyauf::deleteMatrix(transposed_matrix, columns);
    return invalid_memory_usage;
  }

  return 0;
}
