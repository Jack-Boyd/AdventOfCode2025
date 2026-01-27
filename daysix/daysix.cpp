#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using int64 = uint64_t;

struct Grid {
  std::vector<std::string> rows;
  size_t width = 0;
  size_t height = 0;

  bool load(const std::string &filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
      std::cerr << "Error: could not open file " << filename << std::endl;
      return false;
    }

    std::string line;
    while (std::getline(file, line)) {
      width = std::max(width, line.size());
      rows.push_back(line);
    }

    for (auto &row : rows)
      row.resize(width, ' ');

    height = rows.size() - 1;
    return true;
  }

  char at(size_t row, size_t col) const { return rows[row][col]; }

  bool isColumnEmpty(size_t col) const {
    for (size_t r = 0; r < height; ++r)
      if (at(r, col) != ' ')
        return false;
    return true;
  }
};

int64 calculate(const std::vector<int64> &numbers, char op) {
  int64 result = (op == '*') ? 1 : 0;
  for (int64 n : numbers) {
    if (op == '+')
      result += n;
    else if (op == '*')
      result *= n;
  }
  return result;
}

int64 partOne(const Grid &grid) {
  int64 total = 0;
  size_t col = 0;

  while (col < grid.width) {
    if (grid.isColumnEmpty(col)) {
      ++col;
      continue;
    }

    char op = ' ';
    std::vector<int64> numbers;

    while (col < grid.width && !grid.isColumnEmpty(col)) {
      if (grid.at(grid.height, col) != ' ')
        op = grid.at(grid.height, col);

      int64 num = 0;
      for (size_t r = 0; r < grid.height; ++r) {
        char ch = grid.at(r, col);
        if (ch >= '0' && ch <= '9')
          num = num * 10 + (ch - '0');
      }
      numbers.push_back(num);
      ++col;
    }

    std::vector<int64> rowNumbers(grid.height, 0);
    col -= numbers.size();

    for (size_t r = 0; r < grid.height; ++r) {
      int64 num = 0;
      for (size_t c = col; c < grid.width && !grid.isColumnEmpty(c); ++c) {
        char ch = grid.at(r, c);
        if (ch >= '0' && ch <= '9')
          num = num * 10 + (ch - '0');
      }
      rowNumbers[r] = num;
    }

    while (col < grid.width && !grid.isColumnEmpty(col))
      ++col;

    total += calculate(rowNumbers, op);
  }
  return total;
}

int64 partTwo(const Grid &grid) {
  int64 total = 0;
  int col = grid.width - 1;

  while (col >= 0) {
    if (grid.isColumnEmpty(col)) {
      --col;
      continue;
    }

    char op = ' ';
    for (int c = col; c >= 0 && op == ' '; --c)
      if (grid.at(grid.height, c) != ' ')
        op = grid.at(grid.height, c);

    std::vector<int64> numbers;
    while (col >= 0 && !grid.isColumnEmpty(col)) {
      int64 num = 0;
      for (size_t r = 0; r < grid.height; ++r) {
        char ch = grid.at(r, col);
        if (ch >= '0' && ch <= '9')
          num = num * 10 + (ch - '0');
      }
      numbers.push_back(num);
      --col;
    }

    total += calculate(numbers, op);
  }
  return total;
}

int main() {
  Grid grid;
  grid.load("daysix.txt");

  std::cout << "Part One: " << partOne(grid) << std::endl;
  std::cout << "Part Two: " << partTwo(grid) << std::endl;

  return 0;
}
