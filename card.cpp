/*
 * card.cpp
 * CS11 Sushi Go
 */

#include "card.h"

// Card constructor
// Input: sushi name and maki count
// Description: assigns card its type and maki count
// Output: none
Card::Card(string sushiType, int makiCount){
    this->sushiType = sushiType;
    this->makiCount = makiCount;
}

// getSushiType
// Input: none
// Description: gets the sushi card name from the 
//private section of the class
// Output: sushi name
string Card::getSushiType(){
    return sushiType;
}

// getMakiCount
// Input: none
// Description: Gets the maki count of the maki card
// Output: maki count
int Card::getMakiCount(){
    return makiCount;
}