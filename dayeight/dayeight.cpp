#include <algorithm>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

using int64 = uint64_t;

struct JunctionBox {
  int x;
  int y;
  int z;
};

struct Edge {
  float dist;
  size_t i;
  size_t j;

  bool operator<(const Edge &other) const { return dist < other.dist; }
};

struct ConnectionsLookup {
  ConnectionsLookup(size_t n) : parent(n), rank(n, 0), num_circuits(n) {
    for (size_t i = 0; i < n; ++i)
      parent[i] = static_cast<int>(i);
  }

  int find(int x) {
    if (parent[x] != x) {
      parent[x] = find(parent[x]);
    }
    return parent[x];
  }

  bool connect(int x, int y) {
    int px = find(x);
    int py = find(y);
    if (px == py)
      return false;

    if (rank[px] < rank[py])
      std::swap(px, py);
    parent[py] = px;
    if (rank[px] == rank[py])
      rank[px]++;

    num_circuits--;
    return true;
  }

  std::vector<int> parent;
  std::vector<int> rank;
  size_t num_circuits;
};

bool loadBoxes(const std::string &filename, std::vector<JunctionBox> &boxes) {
  std::ifstream inputFile(filename);

  if (!inputFile.is_open()) {
    std::cerr << "Error: could not open file" << filename << std::endl;
    return false;
  }

  std::string line;
  while (std::getline(inputFile, line)) {
    if (line.empty())
      continue;

    JunctionBox j;
    size_t pos = line.find(',');
    size_t pos2 = line.find(',', pos + 1);
    if (pos == std::string::npos || pos2 == std::string::npos) {
      std::cerr << "Invalid line: " << line << std::endl;
      continue;
    }

    j.x = std::stoi(line.substr(0, pos));
    j.y = std::stoi(line.substr(pos + 1, pos2 - (pos + 1)));
    j.z = std::stoi(line.substr(pos2 + 1));
    boxes.push_back(j);
  }
  return true;
}

float distance(const JunctionBox &j1, const JunctionBox &j2) {
  float dx = static_cast<float>(j2.x - j1.x);
  float dy = static_cast<float>(j2.y - j1.y);
  float dz = static_cast<float>(j2.z - j1.z);
  return std::sqrtf(dx * dx + dy * dy + dz * dz);
}

std::vector<Edge> sortedEdges(const std::vector<JunctionBox> &boxes) {
  std::vector<Edge> edges;
  size_t n = boxes.size();

  for (size_t i = 0; i < n; ++i) {
    for (size_t j = 0; j < i; ++j) {
      edges.push_back({distance(boxes[i], boxes[j]), i, j});
    }
  }

  std::sort(edges.begin(), edges.end());
  return edges;
}

int64 partOne(const std::vector<JunctionBox> &boxes,
              const std::vector<Edge> &edges) {
  size_t boxes_count = boxes.size();
  size_t connections = 1000;
  int64 total = 0;

  ConnectionsLookup cl(boxes_count);

  size_t limit = std::min(connections, edges.size());
  for (size_t k = 0; k < limit; ++k) {
    cl.connect(static_cast<int>(edges[k].i), static_cast<int>(edges[k].j));
  }

  std::map<int, int> circuitSizes;
  for (size_t i = 0; i < boxes_count; ++i) {
    circuitSizes[cl.find(static_cast<int>(i))]++;
  }

  std::vector<int> sizes;
  for (const auto &pair : circuitSizes) {
    sizes.push_back(pair.second);
  }
  std::sort(sizes.rbegin(), sizes.rend());

  total = static_cast<int64>(sizes[0]) * sizes[1] * sizes[2];
  return total;
}

int64 partTwo(const std::vector<JunctionBox> &boxes,
              const std::vector<Edge> &edges) {
  size_t boxes_count = boxes.size();
  ConnectionsLookup cl(boxes_count);

  for (const auto &edge : edges) {
    bool merged =
        cl.connect(static_cast<int>(edge.i), static_cast<int>(edge.j));

    if (merged && cl.num_circuits == 1) {
      int64 x1 = boxes[edge.i].x;
      int64 x2 = boxes[edge.j].x;
      return x1 * x2;
    }
  }
  return 0;
}

int main() {
  const std::string filename = "dayeight.txt";
  std::vector<JunctionBox> boxes;
  bool loaded = loadBoxes(filename, boxes);

  std::vector<Edge> edges = sortedEdges(boxes);

  std::cout << "Part One: " << partOne(boxes, edges) << std::endl;
  std::cout << "Part Two: " << partTwo(boxes, edges) << std::endl;

  return 0;
}
