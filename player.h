/*
 * player.h
 * COMP11 Sushi Go
 */

#ifndef PLAYER_H
#define PLAYER_H
#include <iostream>
#include <fstream>
#include <string>
#include "termfuncs.h"
#include "vector.h"
using namespace std;

class Player{
public:
//constructor
Player();

//getters
Vector *getPassingHand();
Vector *getRevealedCards();
int getScore();
int getPuddingCount();

//setters
void setPassingHand(Vector *passingHand);
void setRevealedCards(Vector *passingHand);

//add functions
void addScore(int score);
void addPuddingCount(int puddingCount);

private:
//pointers to hand and kept cards
Vector *passingHand;
Vector *revealedCards;

//score trackers
int score;
int puddingCount;
};

#endif