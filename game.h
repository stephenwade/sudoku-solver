#ifndef GAME_H_
#define GAME_H_

#include <set>
#include <stdexcept>  // for std::invalid_argument
#include <string>

#include "board.h"
#include "operators.h"

class Game {
 public:
  struct BoardValidationResult {
    bool valid;
    bool solved;
    std::string validation_message;

    BoardValidationResult(Board::BoardValidationResult validation_result)
        : valid(validation_result.valid),
          solved(validation_result.solved),
          validation_message(validation_result.validation_message) {}
  };

  struct StepResult {
    bool done;
    std::set<const Cell *> cells_changed;
    std::vector<std::string> change_descriptions;

    static StepResult Step(
        const Operators::OperationResult& operation_result) {
      return StepResult(false, operation_result.cells_changed,
                             operation_result.change_descriptions);
    }

    static StepResult Done() { return StepResult(true); }

   private:
    StepResult(bool done,
               std::set<const Cell *> cells_changed = {},
               const std::vector<std::string>& change_descriptions = {})
        : done(done), cells_changed(cells_changed),
          change_descriptions(change_descriptions) {}
  };

  // Game();
  Game(const std::string& board_filename);

  BoardValidationResult ValidateBoard() const;
  StepResult Step();

  const Board& board() const;

 private:
  Board board_;
};

#endif
