#include <fstream>
#include <iostream>
#include <string>
#include <vector>

std::vector<std::string> loadMap(const std::string &filename) {
  std::vector<std::string> map;
  std::ifstream inputFile(filename);

  if (!inputFile.is_open()) {
    std::cerr << "Error: could not open file" << filename << std::endl;
    return map;
  }

  std::string row;
  while (std::getline(inputFile, row)) {
    map.push_back(row);
  }

  inputFile.close();
  return map;
}

int partOne(const std::vector<std::string> &map) {
  int rolls = 0;
  int rows = map.size();
  if (rows == 0)
    return 0;

  int cols = map[0].size();

  int dx[] = {-1, -1, -1, 0, 0, 1, 1, 1};
  int dy[] = {-1, 0, 1, -1, 1, -1, 0, 1};

  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      if (map[i][j] == '@') {
        int adjacentRolls = 0;

        for (int d = 0; d < 8; d++) {
          int ni = i + dx[d];
          int nj = j + dy[d];

          if (ni >= 0 && ni < rows && nj >= 0 && nj < cols) {
            if (map[ni][nj] == '@') {
              adjacentRolls++;
            }
          }
        }

        if (adjacentRolls < 4) {
          rolls++;
        }
      }
    }
  }

  return rolls;
}

int partTwo(std::vector<std::string> map) {
  int totalRemoved = 0;
  int rows = map.size();
  if (rows == 0)
    return 0;
  int cols = map[0].size();

  int dx[] = {-1, -1, -1, 0, 0, 1, 1, 1};
  int dy[] = {-1, 0, 1, -1, 1, -1, 0, 1};

  while (true) {
    std::vector<std::pair<int, int>> toRemove;

    for (int i = 0; i < rows; i++) {
      for (int j = 0; j < cols; j++) {
        if (map[i][j] == '@') {
          int adjacentRolls = 0;

          for (int d = 0; d < 8; d++) {
            int ni = i + dx[d];
            int nj = j + dy[d];

            if (ni >= 0 && ni < rows && nj >= 0 && nj < cols) {
              if (map[ni][nj] == '@') {
                adjacentRolls++;
              }
            }
          }

          if (adjacentRolls < 4) {
            toRemove.push_back({i, j});
          }
        }
      }
    }

    if (toRemove.empty()) {
      break;
    }

    for (const auto &pos : toRemove) {
      map[pos.first][pos.second] = '.';
    }

    totalRemoved += toRemove.size();
  }

  return totalRemoved;
}
int main() {
  std::string filename = "dayfour.txt";
  std::vector<std::string> map = loadMap(filename);

  std::cout << "Part One: " << partOne(map) << std::endl;
  std::cout << "Part Two: " << partTwo(map) << std::endl;
  return 0;
}
