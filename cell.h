#ifndef CELL_H_
#define CELL_H_

#include <cstddef>  // for std::size_t
#include <set>
#include <string>

class Cell {
 public:
  using CellGuesses = std::set<int>;

  Cell(std::size_t row, std::size_t col);
  Cell(std::size_t row, std::size_t col, int solution);
  Cell(std::size_t row, std::size_t col, const CellGuesses& guesses);

  std::string DescribeLocation() const;

  std::size_t row() const;
  std::size_t col() const;
  int solution() const;
  const CellGuesses& guesses() const;
  bool has_guess(int guess) const;
  bool solved() const;

  void set_solution(int solution);
  void set_guesses(const CellGuesses& guesses);
  void add_guess(int guess);
  void remove_guess(int guess);
  void set_unsolved();

 private:
  void set_location(std::size_t row, std::size_t col);

  std::size_t row_;
  std::size_t col_;
  int solution_;
  CellGuesses guesses_;
  bool solved_;
};

#endif
