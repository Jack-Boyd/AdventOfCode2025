#include <fstream>
#include <iostream>
#include <string>
#include <vector>

struct Move {
  char direction;
  int amount;
};

std::vector<Move> loadMoves(const std::string &filename) {
  std::vector<Move> moves;
  std::ifstream inputFile(filename);

  if (!inputFile.is_open()) {
    std::cerr << "Error: could not open file" << filename << std::endl;
    return moves;
  }

  char dir;
  int amt;

  while (inputFile >> dir >> amt) {
    moves.push_back({dir, amt});
  }

  inputFile.close();
  return moves;
}

int partOne(const std::vector<Move> &moves) {
  int dial = 50;
  int password = 0;

  for (auto move : moves) {
    int change = (move.direction == 'L' ? -1 : 1) * move.amount;
    dial += change;
    dial = ((dial % 100) + 100) % 100;
    if (dial == 0) {
      password++;
    }
  }
  return password;
}

int partTwo(const std::vector<Move> &moves) {
  int dial = 50;
  int password = 0;
  return password;
}

int main() {
  std::string filename = "dayone.txt";
  std::vector<Move> moves = loadMoves(filename);

  int password = partOne(moves);
  std::cout << password << std::endl;

  return 0;
}
