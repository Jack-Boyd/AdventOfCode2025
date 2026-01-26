#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

std::vector<std::string> loadBanks(const std::string &filename) {
  std::vector<std::string> banks;
  std::ifstream inputFile(filename);

  if (!inputFile.is_open()) {
    std::cerr << "Error: could not open file" << filename << std::endl;
    return banks;
  }

  std::string bank;
  while (std::getline(inputFile, bank)) {
    banks.push_back(bank);
  }

  inputFile.close();
  return banks;
}

int partOne(const std::vector<std::string> &banks) {
  int joltage = 0;
  for (const auto &bank : banks) {
    auto maxItTen = std::max_element(bank.begin(), bank.end() - 1);
    int digitTen = *maxItTen - '0';
    int digitTenIndex = maxItTen - bank.begin();

    auto maxItOne =
        std::max_element(bank.begin() + digitTenIndex + 1, bank.end());
    int digitOne = *maxItOne - '0';

    int value = digitTen * 10 + digitOne;
    joltage += value;
  }
  return joltage;
}

unsigned long partTwo(const std::vector<std::string> &banks) {
  unsigned long joltage = 0;
  for (const auto &bank : banks) {
    int toRemove = bank.size() - 12;
    std::string stack;
    for (char digit : bank) {
      while (!stack.empty() && toRemove > 0 && stack.back() < digit) {
        stack.pop_back();
        --toRemove;
      }
      stack.push_back(digit);
    }

    if (stack.size() > 12)
      stack.resize(12);

    unsigned long val = std::stoul(stack);
    joltage += val;
  }
  return joltage;
}

int main() {
  std::string filename = "daythree.txt";
  std::vector<std::string> banks = loadBanks(filename);

  std::cout << "Part One: " << partOne(banks) << std::endl;
  std::cout << "Part Two: " << partTwo(banks) << std::endl;

  return 0;
}
