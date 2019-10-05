#include <algorithm>  // for std::all_of
#include <stdexcept>  // for std::invalid_argument
#include <unordered_set>

#include "board.h"

Board::Board() {
  for (std::size_t i = 1; i <= 9; ++i) {
    BoardRow row;

    for (std::size_t j = 1; j <= 9; ++j) {
      Cell cell{i, j};
      row.push_back(cell);
    }

    data_.push_back(row);
  }

  InitializeBoxLookup();
}

Board::Board(const std::vector<std::vector<int>>& data) {
  const std::string kBoardSizeMessage =
    "There must be exactly 9 rows and columns in the input data.";

  if (data.size() != 9)
    throw std::invalid_argument(kBoardSizeMessage);

  for (std::size_t i = 1; i <= 9; ++i) {
    auto row = data[i - 1];
    if (row.size() != 9)
      throw std::invalid_argument(kBoardSizeMessage);

    BoardRow new_row;

    for (std::size_t j = 1; j <= 9; ++j) {
      int cell_value = row[j - 1];
      Cell cell{i, j, cell_value};
      new_row.push_back(cell);
    }

    data_.push_back(new_row);
  }

  InitializeBoxLookup();
}

Board::BoardValidationResult Board::Validate() const {
  // check that each row, column, and box has no duplicate numbers

  for (std::size_t i = 1; i <= 9; ++i) {
    auto cell_list = row(i);
    if (auto result = ValidateCellList(cell_list); !result.valid) {
      std::string message = "Row " + std::to_string(i) + " is invalid: " +
        "Duplicate " + std::to_string(result.duplicate_solution) + "'s";
      return BoardValidationResult::Invalid(message);
    }
  }

  for (std::size_t i = 1; i <= 9; ++i) {
    auto cell_list = col(i);
    if (auto result = ValidateCellList(cell_list); !result.valid) {
      std::string message = "Column " + std::to_string(i) + " is invalid: " +
        "Duplicate " + std::to_string(result.duplicate_solution) + "'s";
      return BoardValidationResult::Invalid(message);
    }
  }

  for (std::size_t i = 1; i <= 9; ++i) {
    auto cell_list = box(i);
    if (auto result = ValidateCellList(cell_list); !result.valid) {
      std::string message = "Box " + std::to_string(i) + " is invalid: " +
        "Duplicate " + std::to_string(result.duplicate_solution) + "'s";
      return BoardValidationResult::Invalid(message);
    }
  }

  // if execution gets here, there were no duplicate solutions
  // check if it's complete

  for (auto row : data_) {
    if (std::any_of(row.cbegin(), row.cend(),
                    [](const Cell& cell){ return !cell.solved(); }))
      return BoardValidationResult::ValidUnsolved();
  }

  return BoardValidationResult::Solved();
}

std::ostream& operator<<(std::ostream& os, const Board& board) {
  os << board.ToString();
  return os;
}

std::vector<const Cell *> Board::row(std::size_t row_num) const {
  if (row_num < 1 || row_num > 9)
    throw std::invalid_argument("Invalid row number");

  std::vector<const Cell *> result;
  for (std::size_t i = 0; i < 9; ++i)
    result.push_back(&(data_[row_num - 1][i]));

  return result;
}

std::vector<Cell *> Board::row(std::size_t row_num) {
  if (row_num < 1 || row_num > 9)
    throw std::invalid_argument("Invalid row number");

  std::vector<Cell *> result;
  for (std::size_t i = 0; i < 9; ++i)
    result.push_back(&(data_[row_num - 1][i]));

  return result;
}

std::vector<const Cell *> Board::col(std::size_t col_num) const {
  if (col_num < 1 || col_num > 9)
    throw std::invalid_argument("Invalid column number");

  std::vector<const Cell *> result;
  for (std::size_t i = 0; i < 9; ++i)
    result.push_back(&(data_[i][col_num - 1]));

  return result;
}

std::vector<Cell *> Board::col(std::size_t col_num) {
  if (col_num < 1 || col_num > 9)
    throw std::invalid_argument("Invalid column number");

  std::vector<Cell *> result;
  for (std::size_t i = 0; i < 9; ++i)
    result.push_back(&(data_[i][col_num - 1]));

  return result;
}

std::vector<const Cell *> Board::box(std::size_t box_num) const {
  if (box_num < 1 || box_num > 9)
    throw std::invalid_argument("Invalid box number");

  const Cell *box_corner = box_lookup_[box_num - 1];

  std::vector<const Cell *> result;
  for (std::size_t i = 0; i < 9; ++i)
    result.push_back(&(data_[box_corner->row() - 1 + (i / 3)]
                            [box_corner->col() - 1 + (i % 3)]));

  return result;
}

std::vector<Cell *> Board::box(std::size_t box_num) {
  if (box_num < 1 || box_num > 9)
    throw std::invalid_argument("Invalid box number");

  const Cell *box_corner = box_lookup_[box_num - 1];

  std::vector<Cell *> result;
  for (std::size_t i = 0; i < 9; ++i)
    result.push_back(&(data_[box_corner->row() - 1 + (i / 3)]
                            [box_corner->col() - 1 + (i % 3)]));

  return result;
}

Board::CellListValidationResult Board::ValidateCellList(
    std::vector<const Cell *> cell_list) const {
  std::unordered_set<int> solutions_in_list;

  for (const Cell *cell: cell_list) {
    if (cell->solved()) {
      auto insert_result = solutions_in_list.insert(cell->solution());

      // did the insertion fail?
      if (!insert_result.second)
        // if so, there was a duplicate solution in this cell list
        return CellListValidationResult::Invalid(cell->solution());
    }
  }

  // if execution gets here, there were no duplicate solutions
  return CellListValidationResult::Valid();
}

void Board::InitializeBoxLookup() {
  box_lookup_ = {
    &data_[0][0],
    &data_[0][3],
    &data_[0][6],
    &data_[3][0],
    &data_[3][3],
    &data_[3][6],
    &data_[6][0],
    &data_[6][3],
    &data_[6][6]
  };
}
