#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>

int main() {
	std::ifstream input_list("./input1.txt");

	std::vector<int> historian1;
	std::vector<int> historian2;
	
	int tmp_hist1;
	int tmp_hist2;
	
	while(input_list >> tmp_hist1 >> tmp_hist2) {
		historian1.push_back(tmp_hist1);
		historian2.push_back(tmp_hist2);
	}
	
	std::sort(historian1.begin(), historian1.end());
	std::sort(historian2.begin(), historian2.end());

	long long sum{ 0 };
	for(size_t i { 0 }; i < historian2.size(); ++i) {
		sum += std::abs(historian1.at(i) - historian2.at(i));
	}
	
	std::cout << "Part 1: " << sum << std::endl;

  // -- part 2
  
  sum = 0;
  for(const auto& num : historian1) {
    sum += num * std::count(historian2.begin(), historian2.end(), num);
  }

	std::cout << "Part 2: " << sum << std::endl;
}
