#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <utility>
#include <unordered_map>
#include <limits>

std::pair<int, int> parse_rule(const std::string& rule_line) {
  std::stringstream ss(rule_line);
  int first, second;
  char delimeter;
  ss >> first >> delimeter >> second;

  return std::make_pair(first, second);
}

std::vector<int> parse_update(const std::string& update_line) {
  std::vector<int> res;
  std::stringstream ss(update_line);
  std::string number_string;

  while(std::getline(ss, number_string, ',')) {
    res.push_back(std::stoi(number_string));
  }

  return res;
}

std::pair<std::vector<std::pair<int,int>>, std::vector<std::vector<int>>> parse_file(const std::string& file_path) {
  std::string line;
  std::ifstream file(file_path);
  bool is_rule_section{ true };

  std::vector<std::pair<int, int>> rules;
  std::vector<std::vector<int>> updates;

  while(std::getline(file, line)) {
    if(line.size() < 1) is_rule_section = false;
    else if(is_rule_section) rules.push_back(parse_rule(line));
    else updates.push_back(parse_update(line)); 
  }

  return std::make_pair(rules, updates);
}

bool check_update(const std::vector<int>& update, const std::vector<std::pair<int, int>>& rules) {
  for(const auto& rule : rules) {
    const auto first_index { std::find(update.cbegin(), update.cend(), rule.first)  };
    const auto second_index{ std::find(update.cbegin(), update.cend(), rule.second) };

    if (first_index == update.cend() || second_index == update.cend())
      continue;

    if(first_index > second_index)
      return false;
  }
  return true;
}

int get_middle_number(const std::vector<int>& update) {
  size_t middle_index{ update.size() / 2 };
  return update.at(middle_index);
}

void fix_update(std::vector<int>& update, const std::vector<std::pair<int, int>>& rules) {
  std::sort(update.begin(), update.end(), [&rules](int a, int b) {
    const auto it{ std::find(rules.cbegin(), rules.cend(), std::make_pair(a, b)) };
    if(it != rules.cend()) return true;
    else return false;
  });
}

int main() {
  auto[rules, updates]{ parse_file("./input5.txt") };

  int res{ 0 };
  for(const auto& update : updates) {
    if(check_update(update, rules)){
      res += get_middle_number(update);
    }
  }

  std::cout << "Part 1: " << res << std::endl;

  // -- part 2
  res = 0;
  for(auto& update : updates) {
    if(not check_update(update, rules)){
      fix_update(update, rules);
      
      for(auto& num : update) {
        std::cout << num << " ";
      }
      std::cout << std::endl;
      res += get_middle_number(update);
    }
  }

  std::cout << "Part 1: " << res << std::endl;
}
