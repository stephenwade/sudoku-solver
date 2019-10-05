#include <iterator>  // for std::advance
#include <sstream>

#include "board.h"

std::string Board::ToString(
    const std::set<const Cell *>& cells_to_highlight) const {
  // ╔═════╤═════╤═════╦═════╤═════╤═════╦═════╤═════╤═════╗
  // ║     │     │     ║4567 │6    │47   ║2456 │     │467  ║
  // ║  9  │  3  │  1  ║     │     │     ║     │  8  │     ║
  // ╟─────┼─────┼─────╫─────┼─────┼─────╫─────┼─────┼─────╢
  // ║46   │567  │     ║     │136  │     ║3456 │3457 │34567║
  // ║     │     │  2  ║  8  │     │  9  ║     │     │     ║
  // ╟─────┼─────┼─────╫─────┼─────┼─────╫─────┼─────┼─────╢
  // ║46   │567  │     ║     │     │47   ║     │457  │     ║
  // ║     │     │  8  ║  3  │  2  │     ║  1  │     │  9  ║
  // ╠═════╪═════╪═════╬═════╪═════╪═════╬═════╪═════╪═════╣
  // ║128  │1278 │7    ║1269 │     │123  ║3689 │39   │     ║
  // ║     │     │     ║     │  4  │     ║     │     │  5  ║
  // ╟─────┼─────┼─────╫─────┼─────┼─────╫─────┼─────┼─────╢
  // ║     │14   │     ║19   │     │     ║349  │349  │     ║
  // ║  5  │     │  6  ║     │  7  │  8  ║     │     │  2  ║
  // ╟─────┼─────┼─────╫─────┼─────┼─────╫─────┼─────┼─────╢
  // ║     │     │4    ║26   │     │2    ║     │     │468  ║
  // ║  3  │  9  │     ║     │  5  │     ║  7  │  1  │     ║
  // ╠═════╪═════╪═════╬═════╪═════╪═════╬═════╪═════╪═════╣
  // ║1268 │12568│59   ║12479│1389 │12347║23458│23457│13478║
  // ║     │     │     ║     │     │     ║9    │9    │     ║
  // ╟─────┼─────┼─────╫─────┼─────┼─────╫─────┼─────┼─────╢
  // ║128  │128  │     ║12479│189  │     ║2489 │     │1478 ║
  // ║     │     │  3  ║     │     │  5  ║     │  6  │     ║
  // ╟─────┼─────┼─────╫─────┼─────┼─────╫─────┼─────┼─────╢
  // ║     │     │59   ║1249 │1389 │     ║23589│2359 │138  ║
  // ║  7  │  4  │     ║     │     │  6  ║     │     │     ║
  // ╚═════╧═════╧═════╩═════╧═════╧═════╩═════╧═════╧═════╝

  const std::string kCellBorder = "│";
  const std::string kBoxBorder =  "║";
  const std::string kTopLine =
    "╔═════╤═════╤═════╦═════╤═════╤═════╦═════╤═════╤═════╗\n";
  const std::string kCellBorderLine =
    "╟─────┼─────┼─────╫─────┼─────┼─────╫─────┼─────┼─────╢\n";
  const std::string kBoxBorderLine =
    "╠═════╪═════╪═════╬═════╪═════╪═════╬═════╪═════╪═════╣\n";
  const std::string kBottomLine =
    "╚═════╧═════╧═════╩═════╧═════╧═════╩═════╧═════╧═════╝\n";
  const std::string kFormatHighlightBegin = "\033[7m";
  const std::string kFormatHighlightEnd = "\033[27m";

  std::ostringstream result;

  result << kTopLine;

  // each row
  for (std::size_t i = 0; i < data_.size(); ++i) {
    if (i > 0) {
      if (i % 3 == 0)
        result << kBoxBorderLine;
      else
        result << kCellBorderLine;
    }

    // each cell, line 1
    for (std::size_t j = 0; j < data_[i].size(); ++j) {
      const auto& cell = data_[i][j];

      if (j % 3 == 0)
        result << kBoxBorder;
      else
        result << kCellBorder;

      bool highlight = cells_to_highlight.count(&cell) == 1;
      // C++20: bool highlight = cells_to_highlight.contains(&cell);

      if (highlight)
        result << kFormatHighlightBegin;
      result << CellToStringLine1(cell);
      if (highlight)
        result << kFormatHighlightEnd;
    }
    result << kBoxBorder;
    result << '\n';

    // each cell, line 2
    for (std::size_t j = 0; j < data_[i].size(); ++j) {
      const auto& cell = data_[i][j];

      if (j % 3 == 0)
        result << kBoxBorder;
      else
        result << kCellBorder;

      bool highlight = cells_to_highlight.count(&cell) == 1;
      // C++20: bool highlight = cells_to_highlight.contains(cell_ref);

      if (highlight)
        result << kFormatHighlightBegin;
      result << CellToStringLine2(cell);
      if (highlight)
        result << kFormatHighlightEnd;
    }
    result << kBoxBorder;
    result << '\n';
  }

  result << kBottomLine;

  return result.str();
}

std::string Board::CellToStringLine1(const Cell& cell) {
  const int kLineLength = 5;

  if (cell.solved())
    return std::string(kLineLength, ' ');
  
  std::ostringstream result;

  auto guesses = cell.guesses();
  int guesses_this_line = guesses.size();
  if (guesses_this_line > kLineLength)
    guesses_this_line = kLineLength;

  auto it = guesses.begin();
  for (int i = 0; i < guesses_this_line && it != guesses.end(); ++i) {
    int this_guess = *it;
    result << this_guess;
    ++it;
  }

  int spaces_this_line = kLineLength - guesses_this_line;
  for (int i = 0; i < spaces_this_line; ++i)
    result << " ";

  return result.str();
}

std::string Board::CellToStringLine2(const Cell& cell) {
  const int kLineLength = 5;
  const int kSpacesBeforeSolution = 2;
  const int kSpacesAfterSolution = 2;
  const std::string kFormatSolutionBegin = "\033[1m";
  const std::string kFormatSolutionEnd = "\033[22m";

  std::ostringstream result;

  if (cell.solved()) {
    result << std::string(kSpacesBeforeSolution, ' ')
           << kFormatSolutionBegin
           << cell.solution()
           << kFormatSolutionEnd
           << std::string(kSpacesAfterSolution, ' ');

    return result.str();
  }

  auto guesses = cell.guesses();
  int guesses_this_line = guesses.size() - kLineLength;
  if (guesses_this_line < 0)
    guesses_this_line = 0;

  if (guesses_this_line > 0) {
    auto it = guesses.begin();
    std::advance(it, kLineLength);
    for (int i = 0; i < guesses_this_line && it != guesses.end(); ++i) {
      int this_guess = *it;
      result << this_guess;
      ++it;
    }
  }

  int spaces_this_line = kLineLength - guesses_this_line;
  for (int i = 0; i < spaces_this_line; ++i)
    result << " ";

  return result.str();
}
