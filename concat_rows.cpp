#include <cstddef>
#include <iostream>

size_t max(const size_t a, const size_t b)
{
  size_t max_size = 0;
  if (a > b) {
    max_size = a;
  } else {
    max_size = b;
  }
  return max_size;
}

int **concat_rows(const int *const *lhs, size_t n1, size_t m1, const int *const *rhs, size_t n2, size_t m2)
{
  int **matrix = nullptr;
  size_t max_n = max(n1, n2);

  matrix = new int *[max_n];

  for (size_t i = 0; i < max_n; ++i) {
    matrix[i] = nullptr;

    if (max_n == n1 && max_n == n2) {
      matrix[i] = new int[m1 + m2];

      for (size_t j = 0; j < m1; ++j) {
        matrix[i][j] = lhs[i][j];
      }

      for (size_t j = 0; j < m2; ++j) {
        matrix[i][j + m1] = rhs[i][j];
      }

    } else if (max_n == n1) {

      if (i < n2) {
        matrix[i] = new int[m1 + m2];

        for (size_t j = 0; j < m1; ++j) {
          matrix[i][j] = lhs[i][j];
        }

        for (size_t j = 0; j < m2; ++j) {
          matrix[i][j + m1] = rhs[i][j];
        }

      } else {
        matrix[i] = new int[m1];

        for (size_t j = 0; j < m1; ++j) {
          matrix[i][j] = lhs[i][j];
        }
      }

    } else {

      if (i < n1) {
        matrix[i] = new int[m1 + m2];

        for (size_t j = 0; j < m1; ++j) {
          matrix[i][j] = lhs[i][j];
        }

        for (size_t j = 0; j < m2; ++j) {
          matrix[i][j + m1] = rhs[i][j];
        }
      } else {
        matrix[i] = new int[m2];

        for (size_t j = 0; j < m2; ++j) {
          matrix[i][j] = rhs[i][j];
        }
      }
    }
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

int main()
{
  size_t rows1 = 0, columns1 = 0, rows2 = 0, columns2 = 0;

  std::cout << "matrix 1\n";
  std::cin >> rows1 >> columns1;

  int **matrix1 = new int *[rows1]{};

  for (std::size_t i = 0; i < rows1; ++i) {
    matrix1[i] = new int[columns1]{};

    for (std::size_t j = 0; j < columns1; ++j) {
      std::cin >> matrix1[i][j];
    }
  }

  std::cout << "matrix 2\n";
  std::cin >> rows2 >> columns2;

  int **matrix2 = new int *[rows2]{};

  for (std::size_t i = 0; i < rows2; ++i) {
    matrix2[i] = new int[columns2]{};

    for (std::size_t j = 0; j < columns2; ++j) {
      std::cin >> matrix2[i][j];
    }
  }

  int **matrix3 = concat_rows(matrix1, rows1, columns1, matrix2, rows2, columns2);

  for (size_t i = 0; i < (rows2); ++i) {
    for (size_t j = 0; j < (columns1 + columns2); ++j) {
      std::cout << matrix3[i][j] << " ";
    }
    std::cout << "\n";
  }

  for (size_t i = rows2; i < (rows1 + rows2); ++i) {
    for (size_t j = 0; j < (columns1); ++j) {
      std::cout << matrix3[i][j] << " ";
    }
    std::cout << "\n";
  }
}
