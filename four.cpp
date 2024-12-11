#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

std::vector<std::vector<char>> parse_input(const std::string& file_path) {
    std::ifstream input_file(file_path);
    std::vector<std::vector<char>> parsed_data;
    std::string line;

    while (std::getline(input_file, line)) {
        std::vector<char> group;

        for (const char ch : line) {
            group.push_back(ch);
        }
        parsed_data.push_back(group);
    }

    return parsed_data;
}

size_t count_word(const std::vector<std::vector<char>>& word_matrix, const std::string& word) {
  const size_t& rows{ word_matrix.size() };
  const size_t& cols{ word_matrix.at(0).size() };
  size_t count{ 0 };

  const auto is_within_bounds = [&](int x, int y) {
      return x >= 0 && x < word_matrix.size() && y >= 0 && y < word_matrix.at(x).size();
  };

  const std::vector<std::pair<int, int>> directions = {
    {0, 1},   // horizontal right
    {0, -1},  // horizontal left
    {1, 0},   // vertical down
    {-1, 0},  // vertical up
    {1, 1},   // diagonal down-right
    {-1, -1}, // diagonal up-left
    {1, -1},  // diagonal down-left
    {-1, 1}   // diagonal up-right
  };

  const auto check_word = [&](int start_x, int start_y, int dx, int dy) {
    for (int i { 0 }; i < static_cast<int>(word.size()); ++i) {
      int new_x = start_x+(i*dx);
      int new_y = start_y+(i*dy);

      if (!is_within_bounds(new_x, new_y) || word_matrix.at(new_x).at(new_y) != word.at(i)) {
        return false;
      }
    }
    return true;
  };

  for (int x{ 0 }; x < static_cast<int>(word_matrix.size()); ++x) {
    for (int y{ 0 }; y < static_cast<int>(word_matrix.at(x).size()); ++y) {
      for (const auto& [dx, dy] : directions) {
        if (check_word(x, y, dx, dy)) {
          ++count;
        }
      }
    }
  }

  return count;
}

size_t count_cross(const std::vector<std::vector<char>>& word_matrix, const std::string& word) {
  if(word.size() % 2 != 1) {
    return 0; // word has no middle letter
  }

  const size_t& middle_index{ word.size()/2 };

  const size_t& rows{ word_matrix.size() };
  const size_t& cols{ word_matrix.at(0).size() };
  size_t count{ 0 };

  const auto is_within_bounds = [&](int x, int y) {
      return x >= 0 && x < word_matrix.size() && y >= 0 && y < word_matrix.at(x).size();
  };

  const std::vector<std::pair<int, int>> directions = {
    {1, 1},   // diagonal down-right
    {-1, -1}, // diagonal up-left
    {1, -1},  // diagonal down-left
    {-1, 1}   // diagonal up-right
  };

  const auto check_word = [&](int start_x, int start_y, int dx, int dy) {
    for (int i { 0 }; i < static_cast<int>(word.size()); ++i) {
      int new_x = start_x+(i*dx);
      int new_y = start_y+(i*dy);

      if (!is_within_bounds(new_x, new_y) || word_matrix.at(new_x).at(new_y) != word.at(i)) {
        return false;
      }
    }
    return true;
  };
  
  std::vector<std::pair<int, int>> medians;
  for (int x{ 0 }; x < static_cast<int>(word_matrix.size()); ++x) {
    for (int y{ 0 }; y < static_cast<int>(word_matrix.at(x).size()); ++y) {
      for (const auto& [dx, dy] : directions) {
        if (check_word(x, y, dx, dy)) {
          const auto median{ std::make_pair(x+(middle_index*dx), y+(middle_index*dy)) };
          const auto it{ std::find(medians.begin(), medians.end(), median) };
          if(it != medians.end()) {
            medians.erase(it, it+1);
            ++count;
          } else {
            medians.push_back(median);
          }
        }
      }
    }
  }

  return count;
}

int main() {
  const auto word_matrix{ parse_input("./input4.txt") };
  const auto result_one{count_word(word_matrix, std::string("XMAS")) };

  std::cout << "Part1: " << result_one << std::endl;

  const auto result_two{count_cross(word_matrix, std::string("MAS")) };
  std::cout << "Part1: " << result_two<< std::endl;
}
