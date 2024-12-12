#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <sstream>

//----------------------------------------
//
//----------------------------------------
std::vector<std::vector<int>> parse_map(const std::string& file_path) {
  std::ifstream map_file(file_path);
  std::string line;

  std::vector<std::vector<int>> map;

  while(std::getline(map_file, line)) {
    std::vector<int> row;
    for(const char& c : line) {
      row.push_back(static_cast<int>(c - '0')); 
    }
    map.push_back(row);
  }

  return map;
}

//----------------------------------------
//
//----------------------------------------
std::vector<std::pair<int, int>> get_starting_points(const std::vector<std::vector<int>>& map) {
  std::vector<std::pair<int, int>> starting_points;
  for(size_t y{ 0 }; y < map.size(); ++y) {
    for(size_t x{ 0 }; x < map.at(y).size(); ++x) {
      if(map.at(y).at(x) == 0)
        starting_points.emplace_back(x, y);
    }
  }

  return starting_points;
}

//----------------------------------------
//
//----------------------------------------
std::vector<std::pair<int, int>> get_walkable(const std::vector<std::vector<int>>& map, const std::pair<int, int>& starting_point) {
  std::vector<std::pair<int, int>> walkable;

  const auto& current_value{ map.at(starting_point.second).at(starting_point.first) };
  const auto next_value{ current_value + 1};

  std::vector<std::pair<int, int>> possible_directions{
    {1,0}, {0, 1}, {-1, 0}, {0, -1}
  };

  for(const auto& d : possible_directions) {
    std::pair<int, int> new_point(starting_point.first + d.first, starting_point.second + d.second);

    if(new_point.second < 0 || new_point.second >= map.size()) continue;
    if(new_point.first < 0 || new_point.first >= map.at(new_point.second).size()) continue;

    if(map.at(new_point.second).at(new_point.first) == next_value) {
      walkable.push_back(new_point);
    }
  }

  return walkable;
}

//----------------------------------------
//
//----------------------------------------
size_t sum_routes(const std::vector<std::vector<int>>& map, const std::vector<std::pair<int, int>>& starting_points) {
  size_t sum{ 0 };

  for(const auto& starting_point : starting_points) {
    std::vector<std::pair<int, int>> walkable_points= { starting_point };
    std::set<std::pair<int, int>> reachable_tops;
    while(walkable_points.size() > 0) {
      std::vector<std::pair<int, int>> tmp;
      for(const auto& point : walkable_points) {
        const auto& current_value{ map.at(point.second).at(point.first) };
        if(current_value == 9) {
          reachable_tops.insert(point);
          continue;
        }
        auto np{ get_walkable(map, point) };
        tmp.insert(tmp.end(), np.begin(), np.end());
      }
      walkable_points = std::move(tmp);
    }
    sum += reachable_tops.size();
  }

  return sum;
}

//----------------------------------------
//
//----------------------------------------
size_t sum_ratings(const std::vector<std::vector<int>>& map, const std::vector<std::pair<int, int>>& starting_points) {
  size_t sum{ 0 };

  for(const auto& starting_point : starting_points) {
    std::vector<std::pair<int, int>> walkable_points= { starting_point };
    while(walkable_points.size() > 0) {
      std::vector<std::pair<int, int>> tmp;
      for(const auto& point : walkable_points) {
        const auto& current_value{ map.at(point.second).at(point.first) };
        if(current_value == 9) {
          sum++;
          continue;
        }
        auto np{ get_walkable(map, point) };
        tmp.insert(tmp.end(), np.begin(), np.end());
      }
      walkable_points = std::move(tmp);
    }
  }

  return sum;
}


//----------------------------------------
//
//----------------------------------------
int main() {
  const auto map{ parse_map("./input10.txt") };
  
  for(const auto& l : map) {
    for(const auto& n : l) {
      std::cout << n;
    }
    std::cout << '\n';
  }

  const auto starting_points{ get_starting_points(map) };
  size_t total_sum{ sum_routes(map, starting_points) };
  size_t rating_sum{ sum_ratings(map, starting_points) };

  std::cout << "Part 1: " << total_sum << std::endl;
  std::cout << "Part 2: " << rating_sum << std::endl;
}
