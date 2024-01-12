/*
 * card.h
 * COMP11 Sushi Go
 */

#ifndef CARD_H
#define CARD_H
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class Card{
    public:
    //contructor
    Card(string sushiType, int makiCount);

    //getters
    string getSushiType();
    int getMakiCount();

    private:
    //card variables
    string sushiType;
    int makiCount;
};

#endif