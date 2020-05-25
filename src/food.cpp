#include "food.h"
#include <utility>

Food::Food() = default;

Food::Food(kFood type): _type(type){}

Food::~Food(){}

Food::Food(const Food& other){
  _type = other._type;}

Food& Food::operator = (const Food& other){
  if(this == &other) {return *this;}
  _type = other._type;
  return *this;}

Food::Food(Food&& other){
  _type = std::move(other._type);}

Food& Food::operator = (Food&& other){
  if(this == &other) {return *this;}
  _type = std::move(other._type);
  return *this;}
  
void Food::setType(kFood type){
  _type = type;}

const kFood Food::getType() const{
  return _type;}
  
  

  
  