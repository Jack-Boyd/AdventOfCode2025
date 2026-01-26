#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

typedef uint64_t int64;

struct Range {
  int64 start;
  int64 end;
};

bool loadRangesAndNumbers(const std::string &filename,
                          std::vector<Range> &ranges,
                          std::vector<int64> &numbers) {
  std::ifstream inputFile(filename);

  if (!inputFile.is_open()) {
    std::cerr << "Error: could not open file" << filename << std::endl;
    return false;
  }

  std::string line;
  while (std::getline(inputFile, line)) {
    if (line.empty())
      continue;

    size_t dashPos = line.find('-');
    if (dashPos != std::string::npos) {
      Range r;
      r.start = std::stoull(line.substr(0, dashPos));
      r.end = std::stoull(line.substr(dashPos + 1));
      ranges.push_back(r);
    } else {
      numbers.push_back(std::stoull(line));
    }
  }

  return true;
}

int partOne(const std::vector<Range> &ranges,
            const std::vector<int64> &numbers) {
  int freshIds = 0;

  for (const int64 &number : numbers) {
    for (const Range &range : ranges) {
      if (number >= range.start && number <= range.end) {
        freshIds++;
        break;
      }
    }
  }
  return freshIds;
}

int64 partTwo(std::vector<Range> &ranges) {
  if (ranges.empty())
    return 0;

  std::sort(ranges.begin(), ranges.end(),
            [](const Range &a, const Range &b) { return a.start < b.start; });

  std::vector<Range> merged;
  merged.push_back(ranges[0]);

  for (size_t i = 1; i < ranges.size(); ++i) {
    Range &last = merged.back();
    if (ranges[i].start <= last.end + 1) {
      last.end = std::max(last.end, ranges[i].end);
    } else {
      merged.push_back(ranges[i]);
    }
  }

  int64 freshIds = 0;
  for (const Range &range : merged) {
    freshIds += (range.end - range.start + 1);
  }
  return freshIds;
}

int main() {
  std::string filename = "dayfive.txt";
  std::vector<Range> ranges;
  std::vector<int64> numbers;

  bool loaded = loadRangesAndNumbers(filename, ranges, numbers);

  std::cout << "Part One: " << partOne(ranges, numbers) << std::endl;
  std::cout << "Part Two: " << partTwo(ranges) << std::endl;
  return 0;
}
