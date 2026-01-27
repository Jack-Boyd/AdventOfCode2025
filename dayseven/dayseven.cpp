#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using int64 = uint64_t;

bool loadGrid(const std::string &filename, std::vector<std::string> &grid) {
  std::ifstream inputFile(filename);

  if (!inputFile.is_open()) {
    std::cerr << "Error: could not open file" << filename << std::endl;
    return false;
  }

  std::string line;
  while (std::getline(inputFile, line)) {
    if (line.empty())
      continue;

    grid.push_back(line);
  }

  return true;
}

int partOne(const std::vector<std::string> &grid) {
  int splits = 0;
  const int width = grid[0].size();
  std::vector<char> beams(width, 0);

  int start = grid[0].find('S');
  if (start == std::string::npos) {
    return 0;
  }

  beams[start] = 1;

  for (const auto &row : grid) {
    std::vector<char> next = beams;
    for (int pos = 0; pos < width; ++pos) {
      if (!beams[pos])
        continue;

      if (row[pos] != '^')
        continue;

      ++splits;
      next[pos] = 0;
      if (pos > 0 && !next[pos - 1]) {
        next[pos - 1] = 1;
      }
      if (pos + 1 < width && !next[pos + 1]) {
        next[pos + 1] = 1;
      }
    }
    beams.swap(next);
  }

  return splits;
}

int64 partTwo(const std::vector<std::string> &grid) {
  const int height = grid.size();
  const int width = grid[0].size();

  std::vector<int64> ways(width, 0);
  std::vector<int64> next(width, 0);

  int start = grid[0].find('S');
  if (start == std::string::npos) {
    return 0;
  }

  ways[start] = 1;

  for (const auto &row : grid) {
    std::fill(next.begin(), next.end(), 0);

    for (int pos = 0; pos < width; ++pos) {
      if (ways[pos] == 0) {
        continue;
      }

      if (row[pos] == '^') {
        if (pos > 0) {
          next[pos - 1] += ways[pos];
        }
        if (pos + 1 < width) {
          next[pos + 1] += ways[pos];
        }
      } else {
        next[pos] += ways[pos];
      }
    }

    ways.swap(next);
  }

  int64 total = 0;
  for (int64 w : ways) {
    total += w;
  }

  return total;
}

int main() {
  std::string filename = "dayseven.txt";

  std::vector<std::string> grid;
  bool loaded = loadGrid(filename, grid);

  std::cout << "Part One: " << partOne(grid) << std::endl;
  std::cout << "Part Two: " << partTwo(grid) << std::endl;
  return 0;
}
