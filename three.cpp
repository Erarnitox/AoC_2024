#include <iostream>
#include <regex>
#include <fstream>
#include <string>
#include <vector>
#include <utility>

std::string read_file(const std::string& file_path) {
  std::ifstream file(file_path, std::ios::in | std::ios::binary);
  file.seekg(0, std::ios::end);
  std::streamsize file_size = file.tellg();
  file.seekg(0, std::ios::beg);
  std::string file_contents(static_cast<std::size_t>(file_size), '\0');
  file.read(file_contents.data(), file_size);
  return file_contents;
}

std::vector<std::pair<int, int>> find_multiplications(const std::string& input) {
  const std::regex regex_pattern(R"(mul\((\d+),(\d+)\)|do\(\)|don't\(\))");
  std::smatch match;
  std::vector<std::pair<int, int>> results;
  std::string::const_iterator search_start{ input.cbegin() };
  bool enabled{ true };

  while (std::regex_search(search_start, input.cend(), match, regex_pattern)) {
    if(match[0].str().starts_with("don't(")) {
      enabled = false;
    } else if(match[0].str().starts_with("do(")) {
      enabled = true;
    } else if (match[0].str().starts_with("mul(")) {
      const int first_operand { std::stoi(match[1].str()) };
      const int second_operand{ std::stoi(match[2].str()) };

      if(enabled)
        results.emplace_back(first_operand, second_operand);
    }
    search_start = match.suffix().first;
  }
  return results;
}

size_t sum_of_mult(const std::vector<std::pair<int, int>>& input) {
  size_t result{ 0 };
  for(const auto& p : input) {
    result += (p.first*p.second);
  }
  return result;
}

int main() {
  const auto& corrupted_input{ read_file("./input3.txt") };
  const auto& correct_operands{ find_multiplications(corrupted_input) };
  const auto result { sum_of_mult(correct_operands) };

  std::cout << "Result: " << result << std::endl;
}
