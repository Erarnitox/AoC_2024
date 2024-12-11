#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <set>
#include <cmath>

size_t max_x{ 0 };
size_t max_y{ 0 };

//----------------------------------------
//
//----------------------------------------
std::map<char, std::vector<std::pair<int, int>>> parse_map_file(const std::string& file_path) {
  std::ifstream map_file(file_path);
  std::string line;

  std::map<char, std::vector<std::pair<int, int>>> map;
  size_t y{ 0 };
  while(std::getline(map_file, line)) {
    if(max_x == 0) {
      max_x = line.size() - 1;
      max_y = max_x;
    }

    size_t row_count{ 0 };
    for(size_t x{ 0 }; x < line.size(); ++x) {
      if(line.at(x) == '.') continue;

      map[line.at(x)].push_back(std::make_pair(x, y));
    }
    ++y;
  }

  return map;
}

//----------------------------------------
//
//----------------------------------------
int main() {
  const auto map{ parse_map_file("./input8.txt") };

  std::set<std::pair<int, int>> results;

  for(const auto& channel : map) {
    for(const auto& pos1 : channel.second) {
      for(const auto& pos2 : channel.second) {
        if(pos1 == pos2) continue;
        std::pair<int, int> one_to_two(pos2.first - pos1.first, pos2.second - pos1.second);
        
        std::pair<int, int> a1 = pos1;
        while((not (a1.first<0 || a1.first>max_x)) && (not (a1.second<0 || a1.second>max_y))) {
          results.insert(a1);
          a1 = { a1.first - one_to_two.first, a1.second - one_to_two.second };
        } 

        std::pair<int, int> a2 = pos2;
        while((not (a2.first<0 || a2.first>max_x)) && (not (a2.second<0 || a2.second>max_y))) {
          results.insert(a2);
          a2 = { a2.first + one_to_two.first, a2.second + one_to_two.second };
        }
      }
    }
  }

  std::cout << "Locations: " << results.size() << std::endl;
}
