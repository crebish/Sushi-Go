/* vector.h 
 *
 * Richard Townsend & Chance Rebish  4/2/20
 */
#ifndef VECTOR_H 
#define VECTOR_H

#include <fstream>
#include <iostream>
#include <string>
#include "card.h"
#include "vector.h"
using namespace std;

typedef Card * Data;

class Vector {
public:
  Vector();
  ~Vector();
  int size();
  void push_back(Data value);
  void pop_back();
  Data at(int index);
  void insert(int index, Data value);
  void erase(int index);
  //Some additional methods!
  Data front();
  Data back();
  bool empty();
private:
  int capacity; 
  int vSize;    //We can't use the same name as the size() method above!
  Data *buffer; 
  void reserve_more();
};

#endif
