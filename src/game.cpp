#include "game.h"
#include <iostream>
#include <utility>
#include "SDL.h"

Game::Game(std::size_t grid_width, std::size_t grid_height)
    : snake(grid_width, grid_height),
      engine(dev()),
      random_w(0, static_cast<int>(grid_width-1)),
      random_h(0, static_cast<int>(grid_height-1)) {
  PlaceFood();
}

void Game::Run(Controller const &controller, Renderer &renderer,
               std::size_t target_frame_duration) {
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  bool running = true;

  while (running) {
    frame_start = SDL_GetTicks();

    // Input, Update, Render - the main game loop.
    controller.HandleInput(running, snake);
    Update();
    renderer.Render(snake, _foods);

    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000) {
      renderer.UpdateWindowTitle(score, frame_count);
      frame_count = 0;
      title_timestamp = frame_end;
    }

    // If the time for this frame is too small (i.e. frame_duration is
    // smaller than the target ms_per_frame), delay the loop to
    // achieve the correct frame rate.
    if (frame_duration < target_frame_duration) {
      SDL_Delay(target_frame_duration - frame_duration);
    }
  }
}

void Game::PlaceFood() {
  _foods.clear();
  
  while (true) {
    // Check that the location is not occupied before placing food
   // Place good food
    int x, y;
    do{
    x = random_w(engine);
    y = random_h(engine);}
    while(Occupied(x, y));

    std::unique_ptr<Food> good_food = std::make_unique<Food>();

     good_food->food.x = x;
     good_food->food.y = y;
    
    _foods.push_back(std::move(good_food));
    
    // Place bad food
    for (int cnt = 0; cnt < _bad_food_count; cnt++){
      do{
        x = random_w(engine);
        y = random_h(engine);}
        while(Occupied(x, y));

      std::unique_ptr<Food> bad_food = std::make_unique<Food>(kFood::kBad);
      
        bad_food->food.x = x;
        bad_food->food.y = y;
      
      _foods.push_back(std::move(bad_food));
    }
    return;
  }  
 }

bool Game::Occupied(int x, int y){
  if (snake.SnakeCell(x, y)) return true;
  if (!_foods.empty()){
  for (auto const &food : _foods) {
    if (food->food.x == x && food->food.y == y) {
      return true;
    }
  }}
  return false;
}
  
void Game::Update() {
  if (!snake.alive) return;

  snake.Update();

  int new_x = static_cast<int>(snake.head_x);
  int new_y = static_cast<int>(snake.head_y);
  for (auto const &food : _foods){
     if (food->getType() == kFood::kGood && food->food.x == new_x && food->food.y == new_y) {
        score++;
        _bad_food_count = _bad_food_count * 2;
        if (_bad_food_count >= 32) _bad_food_count = 32;        
        PlaceFood();
        // Grow snake and increase speed.
        snake.GrowBody();
        snake.speed += 0.01;
        return;
      }else if (food->getType() == kFood::kBad && food->food.x == new_x && food->food.y == new_y){
        score--;
        //_bad_food_count = _bad_food_count / 2;
        //if (_bad_food_count <= 2) _bad_food_count = 2;
        PlaceFood();
        if (score < 0) {score = 0;}
        snake.ShrinkBody();
        snake.speed -= 0.01;
        if (snake.speed < 0.05) {snake.speed = 0.05;}
        return;
     }
  }
}

int Game::GetScore() const { return score; }
int Game::GetSize() const { return snake.size; }