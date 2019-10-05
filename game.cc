#include <fstream>
#include <sstream>
#include <vector>

#include "game.h"

Game::Game(const std::string& board_filename) {
  std::ifstream fs(board_filename, std::ifstream::in);

  std::vector<std::vector<int>> board_from_file;

  std::string line;
  while (std::getline(fs, line)) {
    std::istringstream ss(line);
    std::vector<int> row;

    std::string word;
    while (ss >> word) {
      int cell = std::stoi(word);

      if (cell < 0 || cell > 9)
        throw std::invalid_argument("Cell must be between 0 and 9");

      row.push_back(cell);
    }

    if (row.size() != 9)
      throw std::invalid_argument("Row must have 9 cells");

    board_from_file.push_back(row);
  }

  if (board_from_file.size() != 9)
    throw std::invalid_argument("Board must have 9 rows");

  board_ = Board(board_from_file);
}

Game::BoardValidationResult Game::ValidateBoard() const {
  return board_.Validate();
}

Game::StepResult Game::Step() {
  if (auto result = Operators::FillInGuesses(board_); result.changed())
    return StepResult::Step(result);

  if (auto result = Operators::SingleGuessRule(board_); result.changed())
    return StepResult::Step(result);

  if (auto result = Operators::HiddenSingleGuessRule(board_);
      result.changed()) {
    return StepResult::Step(result);
  }

  return StepResult::Done();
}

const Board& Game::board() const {
  return board_;
}
