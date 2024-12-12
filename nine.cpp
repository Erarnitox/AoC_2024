#include <iostream>
#include <fstream>
#include <vector>

//----------------------------------------
//
//----------------------------------------
std::vector<std::optional<size_t>> parse_file(const std::string& file_path) {
  std::fstream input_file(file_path);
  std::vector<std::optional<size_t>> file_system;

  size_t count{ 0 };
  size_t file_id{ 0 };

  char n;
  while(input_file >> n) {
    int num { static_cast<int>(n - '0') };

    for(int i{ 0 }; i < num; ++i) {
      if(count%2 == 0) {
        //file
        file_system.push_back(file_id);
      } else {
        //empty space
        file_system.push_back(std::nullopt);
      }
    }
    if(count%2) {
      ++file_id;
    }
    ++count;
  }
  std::cout << std::endl;

  return file_system;
}

//----------------------------------------
//
//----------------------------------------
void frag_compress(std::vector<std::optional<size_t>>& file_system) {
  auto empty_it{ file_system.begin() };
  auto end_it{ file_system.end() };

  while(empty_it < end_it) {
    // find the next empty spot:
    while(empty_it->has_value())
      ++empty_it;

    // find the right most value to fill in
    while(not end_it->has_value())
      --end_it;
    
    if(empty_it < end_it) {
      *empty_it = end_it->value();
      *end_it   = std::nullopt;
    }
  }
}

//----------------------------------------
//
//----------------------------------------
struct File {
    size_t file_id;
    size_t length;
    size_t start_index;
};

//----------------------------------------
//
//----------------------------------------
std::vector<File> parse_files(const std::vector<std::optional<size_t>>& file_system) {
    std::vector<File> files;

    size_t i{ 0 };
    while (i < file_system.size()) {
        if (file_system.at(i).has_value()) {
            const size_t current{ file_system.at(i).value() };
            size_t start{ i };
            while (i < file_system.size() && file_system[i] == current) {
                ++i;
            }
            files.push_back({current, i - start, start});
        } else {
            ++i;
        }
    }

    return files;
}

//----------------------------------------
//
//----------------------------------------
void defrag_compress(std::vector<std::optional<size_t>>& file_system) {
  auto files{ parse_files(file_system) };
  
  std::sort(files.begin(), files.end(), [](const File& a, const File& b) {
    return a.file_id > b.file_id;
  });

  for (const auto& file : files) {
    auto space_start{ file_system.end() };

    // debug output:
    /*
    for(const auto& value : file_system) {
      if(value.has_value()){
        std::cout << value.value();
      } else {
        std::cout << '.';
      }
    }
    std::cout << std::endl;
    */

    for (size_t i{ 0 }; i < file_system.size(); ++i) {
      while(i < file_system.size() && file_system.at(i).has_value()) {
        ++i;
      }

      size_t size{ 0 };
      size_t start{ i };

      if(start > file.start_index) break;

      while(file.length > size && (not file_system.at(i).has_value())){
        ++size;
        ++i;
      }
      
      if(size >= file.length) {
        space_start = (file_system.begin() + start);
        break;
      }
    }

    if (space_start != file_system.end()) {
      std::fill(space_start, space_start + file.length, file.file_id);
      std::fill(file_system.begin() + file.start_index, file_system.begin() + file.start_index + file.length, std::nullopt);
    }
  }
}

//----------------------------------------
//
//----------------------------------------
size_t calc_checksum(const std::vector<std::optional<size_t>>& file_system) {
  size_t sum{ 0 };
  for(size_t i{ 0 }; i < file_system.size(); ++i) {
      sum += i*file_system.at(i).value_or(0);
  }

  return sum;
}

//----------------------------------------
//
//----------------------------------------
int main() {
  auto file_system{ parse_file("./input9.txt") };
  //frag_compress(file_system);
  defrag_compress(file_system);

  size_t checksum{ calc_checksum(file_system) };
  
  std::cout << "Checksum: " << checksum << std::endl;
}
