#include <stdexcept>  // for std::invalid_argument, std::logic_error

#include "cell.h"

Cell::Cell(std::size_t row, std::size_t col) : solution_(0), solved_(false) {
  set_location(row, col);
}

Cell::Cell(std::size_t row, std::size_t col, int solution) {
  set_location(row, col);

  if (solution == 0) {
    solution_ = 0;
    solved_ = false;
  } else {
    set_solution(solution);
  }
}

Cell::Cell(std::size_t row, std::size_t col, const std::set<int>& guesses)
    : solution_(0), solved_(false) {
  set_location(row, col);
  set_guesses(guesses);
}

std::string Cell::DescribeLocation() const {
  char row_char = 'A' + row_ - 1;
  char col_char = 'a' + col_ - 1;
  std::string result;
  result.push_back(row_char);
  result.push_back(col_char);
  return result;
}

std::size_t Cell::row() const {
  return row_;
}

std::size_t Cell::col() const {
  return col_;
}

int Cell::solution() const {
  if (!solved_)
    throw std::logic_error("Cell not solved");

  return solution_;
}

const Cell::CellGuesses& Cell::guesses() const {
  if (solved_)
    throw std::logic_error("Cell solved");

  return guesses_;
}

bool Cell::has_guess(int guess) const {
  if (solved_)
    throw std::logic_error("Cell solved");

  return guesses_.count(guess) == 1;
  // C++20: return guesses_.contains(guess);
}

bool Cell::solved() const {
  return solved_;
}

void Cell::set_solution(int solution) {
  if (solution < 1 || solution > 9) {
    std::string error = std::to_string(solution) + " is an invalid solution";
    throw std::invalid_argument(error);
  }

  solution_ = solution;
  guesses_.clear();
  solved_ = true;
}

void Cell::set_guesses(const Cell::CellGuesses& guesses) {
  for (int guess : guesses) {
    if (guess < 1 || guess > 9) {
      std::string error = std::to_string(guess) + " is an invalid guess";
      throw std::invalid_argument(error);
    }

    guesses_ = guesses;
  }
}

void Cell::add_guess(int guess) {
  if (guess < 1 || guess > 9) {
    std::string error = std::to_string(guess) + " is an invalid guess";
    throw std::invalid_argument(error);
  }

  guesses_.insert(guess);
}

void Cell::remove_guess(int guess) {
  guesses_.erase(guess);
}

void Cell::set_unsolved() {
  solved_ = false;
  solution_ = 0;
}

void Cell::set_location(std::size_t row, std::size_t col) {
  if (row < 1 || row > 9) {
    std::string error = std::to_string(row) + " is an invalid row";
    throw std::invalid_argument(error);
  }

  if (col < 1 || col > 9) {
    std::string error = std::to_string(col) + " is an invalid column";
    throw std::invalid_argument(error);
  }

  row_ = row;
  col_ = col;
}
