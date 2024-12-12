#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <cmath>
#include <format>

//----------------------------------------
//
//----------------------------------------
std::vector<size_t> parse_file(const std::string& file_path) {
  std::ifstream stones_file(file_path);
  std::vector<size_t> stones;

  size_t stone;
  while(stones_file >> stone) {
    stones.push_back(stone);
  }

  return stones;
}

//----------------------------------------
//
//----------------------------------------
void blink(std::vector<size_t>& stones) {
    std::vector<size_t> result;
    result.reserve(stones.size() * 2);

    for (const size_t stone : stones) {
        if (stone == 0) {
            result.emplace_back(1);
        } else {
            const size_t num_digits{ static_cast<size_t>(std::log10(stone)) + 1 };

            if ((num_digits%2) == 0) [[likely]] {
                const size_t divisor{ static_cast<size_t>(std::pow(10, num_digits / 2)) };

                result.emplace_back(stone/divisor);
                result.emplace_back(stone%divisor);
            } else {
                result.emplace_back(stone * 2024);
            }
        }
    }
    stones = std::move(result);
}

//----------------------------------------
//
//----------------------------------------
int main() {
  constexpr size_t BLINK_AMNT{ 25 };
  
  auto stones{ parse_file("./input11.txt") };

  for(int i{ 0 }; i < BLINK_AMNT; ++i) {
    std::cout << std::format("Blinking [{}/{}]...\n", i+1, BLINK_AMNT);
    blink(stones);
    std::cout << std::format("- Blink {}:{} Stones\n\n", i+1, stones.size());
  }

  std::cout << "Stones: " << stones.size() << std::endl;
}

