#ifndef BOARD_H_
#define BOARD_H_

#include <cstdlib>  // for std::size_t
#include <ostream>
#include <set>
#include <string>
#include <utility>  // for std::pair
#include <vector>

#include "cell.h"

class Board {
 public:
  using BoardRow = std::vector<Cell>;
  using BoardData = std::vector<BoardRow>;

  struct BoardValidationResult {
    bool valid;
    bool solved;
    std::string validation_message;

    static BoardValidationResult Invalid(const std::string& validation_message) {
      return BoardValidationResult(false, false, validation_message);
    }

    static BoardValidationResult ValidUnsolved() {
      return BoardValidationResult(true, false);
    }

    static BoardValidationResult Solved() {
      return BoardValidationResult(true, true);
    }

    // top corner of each box
    const std::vector<const Cell *> kBoxLookup;

   private:
    BoardValidationResult(bool valid, bool solved,
                          const std::string& validation_message = "")
        : valid(valid), solved(solved),
          validation_message(validation_message) {}
  };

  Board();
  Board(const std::vector<std::vector<int>>& data);

  BoardValidationResult Validate() const;

  std::vector<const Cell *> row(std::size_t row_num) const;
  std::vector<Cell *> row(std::size_t row_num);

  std::vector<const Cell *> col(std::size_t col_num) const;
  std::vector<Cell *> col(std::size_t col_num);

  std::vector<const Cell *> box(std::size_t box_num) const;
  std::vector<Cell *> box(std::size_t box_num);

  // board_tostring.cpp
  std::string ToString(
      const std::set<const Cell *>& cells_to_highlight = {}) const;

 private:
  struct CellListValidationResult {
    bool valid;
    int duplicate_solution;

    static CellListValidationResult Valid() {
      return CellListValidationResult(true);
    }

    static CellListValidationResult Invalid(int duplicate_solution) {
      return CellListValidationResult(false, duplicate_solution);
    }

   private:
    CellListValidationResult(bool valid, int duplicate_solution = 0)
        : valid(valid), duplicate_solution(duplicate_solution) {}
  };

  CellListValidationResult ValidateCellList(
      std::vector<const Cell *> cell_list) const;

  void InitializeBoxLookup();

  // board_tostring.cpp
  static std::string CellToStringLine1(const Cell& cell);
  static std::string CellToStringLine2(const Cell& cell);

  BoardData data_;

  std::vector<Cell *> box_lookup_;
};

#endif
