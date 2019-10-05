#ifndef OPERATORS_H_
#define OPERATORS_H_

#include <cstdlib>  // for std::size_t
#include <set>
#include <vector>
#include <utility>  // for std::pair

#include "board.h"

class Operators {
 public:
  struct OperationResult {
    std::set<const Cell *> cells_changed;
    std::vector<std::string> change_descriptions;

    OperationResult(const std::set<const Cell *>& cells_changed,
                    const std::string& change_description)
        : cells_changed(cells_changed),
          change_descriptions({change_description}) {}

    OperationResult(const std::set<const Cell *>& cells_changed,
                    const std::vector<std::string>& change_descriptions)
        : cells_changed(cells_changed),
          change_descriptions(change_descriptions) {}

    bool changed() {
      return !cells_changed.empty();
    }
  };

  // fill in empty cells with all possible guesses
  static OperationResult FillInGuesses(Board& board);

  // solve cells with only one guess
  static OperationResult SingleGuessRule(Board& board);

  // solve cells that have the only occurrence of a number in their
  // row, column, or box
  static OperationResult HiddenSingleGuessRule(Board& board);

  // remove invalid guesses
  static void TrimGuesses(Board& board);

 private:
  struct CellChange {
    Cell *cell;
    int solution;

    CellChange(Cell *cell, int solution) : cell(cell),
                                           solution(solution) {}
  };

  Operators() {}  // prevent instantiating this class

  static std::vector<CellChange> HiddenSingleGuessRuleSingleRegion(
      const std::vector<Cell *>& cell_list);

  static void TrimGuessesSingleRegion(const std::vector<Cell *>& cell_list);
};

#endif
