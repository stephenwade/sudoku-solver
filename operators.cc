#include <algorithm>  // for std::sort, std::unique
#include <sstream>
#include <stdexcept>  // for std::logic_error

#include "operators.h"

Operators::OperationResult Operators::FillInGuesses(Board& board) {
  std::set<const Cell *> cells_changed;

  // start by guessing that anything is possible
  for (std::size_t i = 1; i <= 9; ++i) {
    auto row = board.row(i);
    for (std::size_t j = 1; j <= 9; ++j) {
      if (Cell *cell = row[j - 1]; !cell->solved()) {
        if (cell->guesses().empty()) {
          cell->set_guesses({1, 2, 3, 4, 5, 6, 7, 8, 9});
          cells_changed.insert(cell);
        }
      }
    }
  }

  if (!cells_changed.empty())
    TrimGuesses(board);

  return {cells_changed, "Filled in all possible guesses"};
}

Operators::OperationResult Operators::SingleGuessRule(Board& board) {
  std::set<const Cell *> cells_changed;

  for (std::size_t i = 1; i <= 9; ++i) {
    auto row = board.row(i);
    for (std::size_t j = 1; j <= 9; ++j) {
      if (Cell *cell = row[j - 1]; !cell->solved()) {
        auto guesses = cell->guesses();
        if (guesses.size() == 1) {
          int single_guess = *guesses.cbegin();
          cell->set_solution(single_guess);
          cells_changed.insert(cell);
        }
      }
    }
  }

  if (!cells_changed.empty())
    TrimGuesses(board);

  return {cells_changed, "Solved cells with only one guess"};
}

Operators::OperationResult Operators::HiddenSingleGuessRule(Board& board) {
  std::vector<CellChange> changes;
  std::vector<std::string> change_descriptions;

  for (std::size_t i = 1; i <= 9; ++i) {
    auto cell_list = board.row(i);
    auto region_changes = HiddenSingleGuessRuleSingleRegion(cell_list);
    for (auto change : region_changes) {
      changes.push_back(change);

      std::ostringstream description;
      description << "Cell " << change.cell->DescribeLocation()
                  << " had the only " << change.solution << " in its row";
      change_descriptions.push_back(description.str());
    }
  }

  for (std::size_t i = 1; i <= 9; ++i) {
    auto cell_list = board.col(i);
    auto region_changes = HiddenSingleGuessRuleSingleRegion(cell_list);
    for (auto change : region_changes) {
      changes.push_back(change);

      std::ostringstream description;
      description << "Cell " << change.cell->DescribeLocation()
                  << " had the only " << change.solution << " in its column";
      change_descriptions.push_back(description.str());
    }
  }

  for (std::size_t i = 1; i <= 9; ++i) {
    auto cell_list = board.box(i);
    auto region_changes = HiddenSingleGuessRuleSingleRegion(cell_list);

    for (auto change : region_changes) {
      changes.push_back(change);

      std::ostringstream description;
      description << "Cell " << change.cell->DescribeLocation()
                  << " had the only " << change.solution << " in its box";
      change_descriptions.push_back(description.str());
    }
  }

  std::set<const Cell *> cells_changed;
  for (auto change : changes) {
    change.cell->set_solution(change.solution);
    cells_changed.insert(change.cell);
  }

  if (!cells_changed.empty())
    TrimGuesses(board);

  // sort change_descriptions by row and column
  std::sort(change_descriptions.begin(), change_descriptions.end(),
            [](const std::string& a, const std::string& b) {
              char row_a = a[4];
              char row_b = b[4];
              if (row_a < row_b)
                return true;
              if (row_a > row_b)
                return false;

              char col_a = a[13];
              char col_b = b[13];
              return col_a < col_b;
            });

  // remove multiple change_descriptions about the same cell
  auto last = std::unique(change_descriptions.begin(),
                          change_descriptions.end(),
                          [](const std::string& a, const std::string& b) {
                            char row_a = a[4];
                            char row_b = b[4];
                            if (row_a != row_b)
                              return false;

                            char col_a = a[13];
                            char col_b = b[13];
                            return col_a == col_b;
                          });
  change_descriptions.erase(last, change_descriptions.end());

  return {cells_changed, change_descriptions};
}

void Operators::TrimGuesses(Board& board) {
  for (std::size_t i = 1; i <= 9; ++i) {
    auto cell_list = board.row(i);
    TrimGuessesSingleRegion(cell_list);
  }

  for (std::size_t i = 1; i <= 9; ++i) {
    auto cell_list = board.col(i);
    TrimGuessesSingleRegion(cell_list);
  }

  for (std::size_t i = 1; i <= 9; ++i) {
    auto cell_list = board.box(i);
    TrimGuessesSingleRegion(cell_list);
  }
}

std::vector<Operators::CellChange> Operators::HiddenSingleGuessRuleSingleRegion(
    const std::vector<Cell *>& cell_list) {
  std::vector<CellChange> changes;

  for (int guess = 1; guess <= 9; ++guess) {
    std::vector<Cell *> cells_with_this_guess;
    for (std::size_t i = 1; i <= 9; ++i) {
      if (Cell *cell = cell_list[i - 1]; !cell->solved() && cell->has_guess(guess))
        cells_with_this_guess.push_back(cell);
    }

    if (cells_with_this_guess.size() == 1) {
      changes.push_back({cells_with_this_guess[0], guess});
    }
  }

  return changes;
}

void Operators::TrimGuessesSingleRegion(const std::vector<Cell *>& cell_list) {
  std::vector<int> solutions;
  for (Cell *cell : cell_list) {
    if (cell->solved())
      solutions.push_back(cell->solution());
  }

  for (Cell *cell : cell_list) {
    if (!cell->solved()) {
      for (int solution : solutions)
        cell->remove_guess(solution);
      if (cell->guesses().empty())
        throw std::logic_error("Cell has no possible guesses");
    }
  }
}
