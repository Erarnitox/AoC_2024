#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include <algorithm>
#include <format>

//----------------------------------------
//
//----------------------------------------
struct Vector2 {
  int x, y;

  void rotate() {
    int tmp{ this->x };
    this->x = -this->y; 
    this->y = tmp;
  };

  bool operator==(const Vector2& other) const {
    return(this->x == other.x && this->y == other.y);
  }

  bool operator!=(const Vector2& other) const {
    return not(*this == other);
  }
};

//----------------------------------------
//
//----------------------------------------
class SimulationEntity{
public:
  bool done = false;
  std::shared_ptr<std::vector<std::vector<char>>> map_data;
  Vector2 position;
  Vector2 direction;

  virtual void init(std::shared_ptr<std::vector<std::vector<char>>> map_data) = 0;
  virtual void update() = 0;
  virtual int get_count() {
    return 0;
  }

  virtual ~SimulationEntity() = default;
};

//----------------------------------------
//
//----------------------------------------
class Guard : public SimulationEntity {
  int count = 0;
  int doubles = 0;
public:
  void init(std::shared_ptr<std::vector<std::vector<char>>> map_data){
    count = 0;
    doubles = 0;

    const static char gurad_char{ '^' };
    this->map_data = map_data;
    
    for(auto y{ 0 }; y < map_data->size(); ++y) {
      for(auto x{ 0 }; x < map_data->at(y).size(); ++x) {
        if(map_data->at(y).at(x) == gurad_char) {
          this->position = { x, y };
        }
      }
    }

    this->direction = { 0, -1 };
  }

  int get_count() {
    return this->count;
  }


  void update() {
    if(done) return;
    const Vector2 new_position{ this->position.x + this->direction.x, this->position.y + this->direction.y };

    if(map_data->size() <= new_position.y || new_position.y < 0) {
      this->done = true;
      return;
    }

    if(map_data->at(new_position.y).size() <= new_position.x || new_position.x < 0) {
      this->done = true;
      return;
    }

    if(map_data->at(new_position.y).at(new_position.x) == '#') {
      this->direction.rotate();
      return;
    }
    
    this->position = new_position;
    
    /* Part of part1
    if(this->map_data->at(new_position.y).at(new_position.x) == '+') {
      ++this->doubles;
    } else {
      this->map_data->at(new_position.y).at(new_position.x) = '+'; 
      ++this->count;
    }
    */
  }
};

//----------------------------------------
//
//----------------------------------------
class Player : public SimulationEntity {
  void init(std::shared_ptr<std::vector<std::vector<char>>> map_data) {

  }

  void update() {

  }
};

//----------------------------------------
//
//----------------------------------------
std::vector<std::vector<char>> parse_map_file(const std::string& map_file) {
  std::ifstream map_stream(map_file);
  std::string line;

  std::vector<std::vector<char>> map_vector;
  while(std::getline(map_stream, line)) {
    std::vector<char> line_vector;
    for(const auto& c : line) {
      line_vector.push_back(c);
    }
    map_vector.push_back(line_vector);
  }

  return map_vector;
}

//----------------------------------------
//
//----------------------------------------
bool has_loop(std::shared_ptr<std::vector<std::vector<char>>> map) {
  Guard g;
  g.init(map);

  std::vector<std::pair<Vector2, Vector2>> have_been_here;

  while(not g.done) {
    g.update();

    if(not g.done){
      std::pair<Vector2, Vector2> pos_dir{ g.position, g.direction };
      if(std::find(have_been_here.cbegin(), have_been_here.cend(), pos_dir) == have_been_here.cend()) {
        have_been_here.push_back(pos_dir);
      } else {
        return true;
      }
    }
  }
  return false;
}

//----------------------------------------
//
//----------------------------------------
size_t find_possible_blocks_count(std::shared_ptr<std::vector<std::vector<char>>> map) {
  size_t count{ 0 };
  const size_t total{ map->size() * map->at(0).size() };
  size_t result{ 0 };
  for(size_t y{ 0 }; y < map->size(); ++y) {
    for(size_t x{ 0 }; x < map->at(y).size(); ++x) {
      // Debug output
      if(count % 100 == 0)
        std::cout << std::format("[{}/{}] - {}% done!\n", count, total, ((float)count/(float)total)*100.00f);
      ++count;

      if(map->at(y).at(x) == '#') continue;
      if(map->at(y).at(x) == '^') continue;

      char tmp{ map->at(y).at(x) };
      map->at(y).at(x) = '#';
      bool is_looping = has_loop(map);
      map->at(y).at(x) = tmp;

      if(is_looping) {
        ++result;
      }
    }
  }
  return result;
}

//----------------------------------------
//
//----------------------------------------
int main(){
  
  const auto map{ std::make_shared<std::vector<std::vector<char>>>(parse_map_file("./input6.txt")) };

  /* Part1
  Guard g;
  g.init(map);

  while(not g.done) {
    g.update(); 
  }

  std::cout << "Part1: " << g.get_count() << std::endl;
  */

  std::cout << "Part2: " << find_possible_blocks_count(map) << std::endl;
}

