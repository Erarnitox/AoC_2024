#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <string>
#include <sstream>
#include <algorithm>
#include <cmath>

//----------------------------------------
//
//----------------------------------------
std::vector<std::pair<size_t, std::vector<size_t>>> parse_file(const std::string& file_path) {
  std::ifstream input_file(file_path);
  std::string line;
  std::vector<std::pair<size_t, std::vector<size_t>>> equations;

  while(std::getline(input_file, line)) {
    std::istringstream line_stream(line);

    size_t result;
    char c;
    line_stream >> result >> c;

    std::vector<size_t> operands;
    size_t operand;
    while(line_stream >> operand) {
      operands.push_back(operand);
    }

    equations.push_back(std::make_pair(result, operands));
  }
  return equations;
}

//----------------------------------------
//
//----------------------------------------
size_t round_up_10(size_t number) {
    if(number == 1) return 10;
    double log10_val{ std::log10(static_cast<double>(number)) };
    size_t next_power{ static_cast<size_t>(std::ceil(log10_val)) };
    return static_cast<size_t>(std::pow(10, next_power));
}

//----------------------------------------
//
//----------------------------------------
bool is_possible(const std::pair<size_t, std::vector<size_t>>& equation) {
  size_t result{ equation.first };
  for(auto i{ equation.second.rbegin() }; i != equation.second.rend()-1; ++i) {
    size_t num{ *i };
    size_t gp{ round_up_10(num) };

    if((result - num) % gp == 0) {
      std::vector<size_t> sub_vector(i+1, equation.second.rend());
      std::reverse(sub_vector.begin(), sub_vector.end());

      if((result%num==0) && is_possible(std::make_pair((result / num), sub_vector)))
        return true; 
      else if(is_possible(std::make_pair((result - num), sub_vector)))
        return true; 
      else 
        result /= gp;
    } else if(result % num == 0) {
      std::vector<size_t> sub_vector(i+1, equation.second.rend());
      std::reverse(sub_vector.begin(), sub_vector.end());
      
      if(is_possible(std::make_pair((result - num), sub_vector)))
        return true; 
      else
        result /= num;
    } else {
      result -= num;
    }
  }
  const auto& last_num{ equation.second.at(0) };
  return (result == last_num);
}

//----------------------------------------
//
//----------------------------------------
int main() {
  const auto& equations{ parse_file("./input7.txt") };

  size_t result{ 0 };
  for(const auto& equation : equations) {
    if(is_possible(equation)){
      result += equation.first;
    }
  }
  std::cout << "Result: " << result << std::endl;
}
