#ifndef FOOD_H
#define FOOD_H

#include "SDL.h"

enum class kFood{kGood, kBad};

class Food{
  public:
    Food();
    Food(kFood type);
    ~Food();
  
    Food(const Food& other);
    Food& operator = (const Food& other);
    Food(Food&& other);
    Food& operator = (Food&& other);
  
    void setType(kFood type);
    const kFood getType() const;
  
    SDL_Point food;
   
  private:
    kFood _type = kFood::kGood;
};

#endif