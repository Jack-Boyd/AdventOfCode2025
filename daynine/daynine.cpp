#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

using int64 = uint64_t;

struct Coordinate {
  int x;
  int y;
};

bool loadCoords(const std::string &filename, std::vector<Coordinate> &coords) {
  std::ifstream inputFile(filename);

  if (!inputFile.is_open()) {
    std::cerr << "Error: could not open file" << filename << std::endl;
    return false;
  }

  std::string line;
  while (std::getline(inputFile, line)) {
    if (line.empty())
      continue;
    size_t comma = line.find(',');
    Coordinate coord;
    coord.x = std::stoi(line.substr(0, comma));
    coord.y = std::stoi(line.substr(comma + 1));
    coords.push_back(coord);
  }
  return true;
}

int64 getArea(const Coordinate &c1, const Coordinate &c2) {
  int64 side_x = std::abs(c1.x - c2.x) + 1;
  int64 side_y = std::abs(c1.y - c2.y) + 1;
  return side_x * side_y;
}

int64 partOne(const std::vector<Coordinate> &coords) {
  int64 max_area = 0;
  size_t coords_size = coords.size();

  for (size_t i = 0; i < coords_size; ++i) {
    for (size_t j = i + 1; j < coords_size; ++j) {
      int64 area = getArea(coords[i], coords[j]);
      max_area = std::max(max_area, area);
    }
  }
  return max_area;
}

bool isOnBoundary(int px, int py, const std::vector<Coordinate> &polygon) {
  size_t polygon_size = polygon.size();

  for (size_t i = 0; i < polygon_size; ++i) {
    const Coordinate &a = polygon[i];
    const Coordinate &b = polygon[(i + 1) % polygon_size];

    if (a.x == b.x && px == a.x) {
      if (py >= std::min(a.y, b.y) && py <= std::max(a.y, b.y))
        return true;
    } else if (a.y == b.y && py == a.y) {
      if (px >= std::min(a.x, b.x) && px <= std::max(a.x, b.x))
        return true;
    }
  }
  return false;
}

bool isInsidePolygon(int px, int py, const std::vector<Coordinate> &polygon) {
  int crossings = 0;
  size_t polygon_size = polygon.size();

  for (size_t i = 0; i < polygon_size; ++i) {
    const Coordinate &a = polygon[i];
    const Coordinate &b = polygon[(i + 1) % polygon_size];

    if (a.x == b.x && a.x > px) {
      int min_y = std::min(a.y, b.y);
      int max_y = std::max(a.y, b.y);
      if (py >= min_y && py < max_y)
        crossings++;
    }
  }
  return (crossings % 2) == 1;
}

bool isInsideOrOnBoundary(int px, int py,
                          const std::vector<Coordinate> &polygon) {
  return isOnBoundary(px, py, polygon) || isInsidePolygon(px, py, polygon);
}

// This takes ages due to high time complexity
int64 partTwo(const std::vector<Coordinate> &coords) {
  int64 max_area = 0;
  size_t coords_size = coords.size();

  std::vector<int> xs, ys;
  for (size_t i = 0; i < coords_size; ++i) {
    xs.push_back(coords[i].x);
    ys.push_back(coords[i].y);
  }

  std::sort(xs.begin(), xs.end());
  std::sort(ys.begin(), ys.end());
  xs.erase(std::unique(xs.begin(), xs.end()), xs.end());
  ys.erase(std::unique(ys.begin(), ys.end()), ys.end());

  std::vector<int> check_xs = xs;
  std::vector<int> check_ys = ys;
  size_t xs_size = xs.size();
  size_t ys_size = ys.size();

  for (size_t i = 0; i + 1 < xs_size; ++i)
    check_xs.push_back((xs[i] + xs[i + 1]) / 2);
  for (size_t i = 0; i + 1 < ys_size; ++i)
    check_ys.push_back((ys[i] + ys[i + 1]) / 2);

  std::sort(check_xs.begin(), check_xs.end());
  std::sort(check_ys.begin(), check_ys.end());

  std::map<std::pair<int, int>, bool> inside;
  for (size_t i = 0; i < check_xs.size(); ++i) {
    for (size_t j = 0; j < check_ys.size(); ++j) {
      inside[{check_xs[i], check_ys[j]}] =
          isInsideOrOnBoundary(check_xs[i], check_ys[j], coords);
    }
  }

  for (size_t i = 0; i < coords_size; ++i) {
    for (size_t j = i + 1; j < coords_size; ++j) {
      int x1 = std::min(coords[i].x, coords[j].x);
      int x2 = std::max(coords[i].x, coords[j].x);
      int y1 = std::min(coords[i].y, coords[j].y);
      int y2 = std::max(coords[i].y, coords[j].y);

      bool valid = true;
      for (size_t xi = 0; xi < check_xs.size() && valid; ++xi) {
        int x = check_xs[xi];
        if (x < x1)
          continue;
        if (x > x2)
          break;

        for (size_t yi = 0; yi < check_ys.size(); ++yi) {
          int y = check_ys[yi];
          if (y < y1)
            continue;
          if (y > y2)
            break;

          if (!inside[{x, y}]) {
            valid = false;
            break;
          }
        }
      }

      if (valid) {
        int64 area = getArea(coords[i], coords[j]);
        max_area = std::max(max_area, area);
      }
    }
  }
  return max_area;
}

int main() {
  std::string filename = "daynine.txt";
  std::vector<Coordinate> coords;

  bool loaded = loadCoords(filename, coords);

  std::cout << "Part One: " << partOne(coords) << std::endl;
  std::cout << "Part Two: " << partTwo(coords) << std::endl;
  return 0;
}
