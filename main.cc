#include <iostream>
#include <limits>  // for std::numeric_limits
#include <sstream>

#include "game.h"

void output_board(const Board& board,
                  std::set<const Cell *> cells_changed,
                  std::string message) {
  std::cout << board.ToString(cells_changed) << '\n'
            << message << '\n';

  std::cout << "Press Return to continue...";
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int main(int argc, char const *argv[]) {
  const int kSuccess = 0;
  const int kUnableToSolve = 1;
  const int kInvalidBoard = 2;
  const int kNoBoard = 3;

  if (argc < 2) {
    std::cout << "Call the program with a game board file.\n";
    std::cout << "Example:\n";
    std::cout << "  " << argv[0] << " board.txt\n";
    return kNoBoard;
  }

  Game game = Game(argv[1]);

  output_board(game.board(), {}, "Initial board\n");

  if (auto result = game.ValidateBoard(); !result.valid) {
    std::cout << result.validation_message << "\n";
    return kInvalidBoard;
  }

  while (true) {
    auto result = game.Step();
    if (result.done)
      break;

    std::ostringstream description;
    for (std::string change_description : result.change_descriptions)
      description << change_description << '\n';
    output_board(game.board(), result.cells_changed, description.str());
  }

  if (auto result = game.ValidateBoard(); result.solved) {
    return kSuccess;
  } else {
    return kUnableToSolve;
  }
}
