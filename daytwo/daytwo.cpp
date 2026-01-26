#include <fstream>
#include <iostream>
#include <string>
#include <vector>

std::vector<std::string> loadIds(const std::string &filename) {
  std::vector<std::string> ids;
  std::ifstream inputFile(filename);

  if (!inputFile.is_open()) {
    std::cerr << "Error: could not open file" << filename << std::endl;
    return ids;
  }

  std::string id;
  while (std::getline(inputFile, id, ',')) {
    ids.push_back(id);
  }

  inputFile.close();
  return ids;
}

bool isRepeatedSequenceSingle(long num) {
  auto s = std::to_string(num);
  auto length = s.size();
  if (length % 2 != 0)
    return false;

  auto half = length / 2;
  return s.substr(0, half) == s.substr(half);
}

long partOne(const std::vector<std::string> &ids) {
  long invalidIds = 0;
  for (const auto &id : ids) {
    const auto pos = id.find('-');
    long start = std::stol(id.substr(0, pos));
    long end = std::stol(id.substr(pos + 1));
    long idx = 0;
    for (long x = start; x <= end; ++x) {
      if (isRepeatedSequenceSingle(x))
        invalidIds += x;
    }
  }

  return invalidIds;
}

bool isRepeatedSequence(long num) {
  if (num < 10 && num > -10)
    return false;
  auto s = std::to_string(num);
  auto length = s.size();

  for (size_t l = 1; l * 2 <= length; ++l) {
    if (length % l != 0)
      continue;

    bool repeated = true;
    for (size_t i = l; i < length; ++i) {
      if (s[i] != s[i - l]) {
        repeated = false;
        break;
      }
    }
    if (repeated)
      return true;
  }
  return false;
}

long partTwo(const std::vector<std::string> &ids) {
  long invalidIds = 0;
  for (const auto &id : ids) {
    const auto pos = id.find('-');
    long start = std::stol(id.substr(0, pos));
    long end = std::stol(id.substr(pos + 1));

    for (long x = start; x <= end; ++x) {
      if (isRepeatedSequence(x))
        invalidIds += x;
    }
  }
  return invalidIds;
}

int main() {
  std::string filename = "daytwo.txt";
  std::vector<std::string> ids = loadIds(filename);

  std::cout << "Part One: " << partOne(ids) << std::endl;
  std::cout << "Part Two: " << partTwo(ids) << std::endl;

  return 0;
}
