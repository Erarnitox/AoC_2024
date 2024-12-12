#include <iostream>
#include <fstream>
#include <unordered_map>
#include <string>
#include <cmath>

//----------------------------------------
//
//----------------------------------------
std::pair<size_t, size_t> divmod(size_t num, size_t divisor) {
    return { num / divisor, num % divisor };
}

//----------------------------------------
//
//----------------------------------------
int main() {
    std::ifstream infile("./input11.txt");
    std::unordered_map<size_t, size_t> stones;
    int num;

    while (infile >> num) {
        ++stones[num];
    }

    for (size_t blinks{ 0 }; blinks < 75; ++blinks) {
        std::unordered_map<size_t, size_t> new_stones;

        for (const auto& [n, num_stone] : stones) {
            std::string n_str{ std::to_string(n) };
            size_t mid{ n_str.length() / 2 };
            size_t rem{ n_str.length() % 2 };

            if (n == 0) {
                new_stones[1] += num_stone;
            } else if (rem) {
                new_stones[2024 * n] += num_stone;
            } else {
                auto [left, right]{ divmod(n, static_cast<int>(std::pow(10, mid))) };
                new_stones[left] += num_stone;
                new_stones[right] += num_stone;
            }
        }
        stones = new_stones;

        if ((blinks+1) == 25 || (blinks+1) == 75) {
            size_t sum = 0;
            for (const auto& [n, count] : new_stones) {
                sum += count;
            }
            std::cout << "Sum: " << sum << std::endl;
        }
    }
    return 0;
}
