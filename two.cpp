#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>

// -- functions for stage 2
// Function to parse the input file into a vector of integer vectors
std::vector<std::vector<int>> parseInput(const std::string& filename) {
    std::ifstream input_file(filename);
    std::vector<std::vector<int>> parsed_data;
    std::string line;

    while (std::getline(input_file, line)) {
        std::istringstream line_stream(line);
        std::vector<int> group;
        int num;

        while (line_stream >> num) {
            group.push_back(num);
        }
        parsed_data.push_back(group);
    }

    return parsed_data;
}

bool isValidSequence(const std::vector<int>& group) {
    for (size_t i = 1; i < group.size(); ++i) {
        if (std::abs(group[i] - group[i - 1]) < 1 || std::abs(group[i] - group[i - 1]) > 3) {
            return false;
        }
    }
    return true;
}

// Function to check if removing a single level makes the sequence valid
bool checkDampener(const std::vector<int>& group) {
    for (size_t i = 0; i < group.size(); ++i) {
        std::vector<int> newGroup = group;
        newGroup.erase(newGroup.begin() + i);

        if (std::is_sorted(newGroup.begin(), newGroup.end()) ||
            std::is_sorted(newGroup.begin(), newGroup.end(), std::greater<>())) {
            if (isValidSequence(newGroup)) {
                return true;
            }
        }
    }
    return false;
}

// Main processing function
int countSafeGroups(const std::vector<std::vector<int>>& parsed, bool isFirst) {
    int count = 0;

    for (const auto& group : parsed) {
        // Check if the group is sorted (ascending or descending)
        if (!std::is_sorted(group.begin(), group.end()) &&
            !std::is_sorted(group.begin(), group.end(), std::greater<>())) {
            if (isFirst) continue;
            if (checkDampener(group)) {
                ++count;
            }
            continue;
        }

        // Check if the group is a valid sequence or can be made valid
        if (isValidSequence(group)) {
            ++count;
        } else if (!isFirst && checkDampener(group)) {
            ++count;
        }
    }

    return count;
}

std::vector<int> parse_line(const std::string& line) {
    std::istringstream line_stream(line);
    std::vector<int> levels;
    int level;

    while (line_stream >> level) {
        levels.push_back(level);
    }
    return levels;
}

bool process_levels(std::vector<int>& levels) {
    int direction = 0;
    int last_level = 0;
    bool has_dampener = true;

    for (size_t i = 0, tok_count = 0; i < levels.size();) {
        int next_level = levels[i];
        bool switched_dir = ((direction < 0) xor ((next_level - last_level) < 0));
        const int tmp_dir = direction;
        direction = next_level - last_level;

        if ((tok_count > 0 && (std::abs(direction) < 1 || std::abs(direction) > 3)) ||
            (tok_count > 1 && switched_dir)) {
            if (has_dampener) {
                has_dampener = false;

                if (tok_count == 0) {
                    if (levels.size() > 1 && std::abs(levels[1] - levels[0]) > 3) {
                        levels.erase(levels.begin()); 
                    } else {
                        levels.erase(levels.begin() + 1);
                    }
                } else {
                    levels.erase(levels.begin() + i);
                }

                direction = (tok_count > 0) ? tmp_dir : 0;
                continue;
            } else {
                return false;
            }
        }

        last_level = next_level;
        ++i;
        ++tok_count;
    }

    return true; // Line is safe
}

// Function to process the entire input file and count safe lines
int count_safe_lines(const std::string& filename) {
    std::ifstream input_file(filename);

    int safe_counter = 0;
    std::string line;

    while (std::getline(input_file, line)) {
        auto levels = parse_line(line);
        if (process_levels(levels)) {
            ++safe_counter;
        }
    }

    return safe_counter;
}

int main() {
  std::ifstream input_file("./input2.txt");

  size_t safe_counter{ 0 };
  
  std::string line;
  while(std::getline(input_file, line)) {
    std::istringstream line_stream(line);
    
    int next_level{ 0 };
    int last_level{ 0 };
    int direction{ 0 };

    bool is_safe{ true };
    int tok_count{ 0 };
    bool switched_dir{ false };

    while(line_stream >> next_level) { 
      switched_dir = ((direction<0) xor ((next_level - last_level)<0));
      direction = next_level - last_level;

      if((tok_count>0 && ((std::abs(direction)<1) || (std::abs(direction)>3))) 
      || (tok_count>1 && switched_dir)) {
        is_safe = false;
        break;
      }
      
      // set data for the next level
      last_level = next_level;
      ++tok_count; 
    }
    if(is_safe) ++safe_counter;
  }

  std::cout << "Stage 1: " << safe_counter << "\n";

  // -- stage 2
  auto parsed = parseInput("./input2.txt");
  int safe_count = countSafeGroups(parsed, false);
  std::cout << "Stage 2: " << safe_count << "\n";
}
