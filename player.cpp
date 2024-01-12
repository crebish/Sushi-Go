/*
 * player.cpp
 * CS11 Sushi Go
 */

#include "player.h"

// Player constructor
// Input: none
// Description: intializes score and pudding count
// Output: none
Player::Player(){
    score = 0;
    puddingCount = 0;
}

// getPassingHand
// Input: none
// Description: returns a pointer to the passingHand
// Output: passingHand
Vector *Player::getPassingHand(){
    return passingHand;
}

// getRevealedCard
// Input: none
// Description: returns a pointer to the revealedCards
// Output: passingHand
Vector *Player::getRevealedCards(){
    return revealedCards;
}



// getScore
// Input: none
// Description: retrieves the score of the player
// Output: score
int Player::getScore(){
    return score;
}

// getPuddingCount
// Input: none
// Description: retrieves the pudding count of the player
// Output: puddingCount
int Player::getPuddingCount(){
    return puddingCount;
}

// setPassingHand
// Input: a vector pointer to a new hand
// Description: sets passingHand to a new hand
// Output: none
void Player::setPassingHand(Vector *passingHand){
    this->passingHand = passingHand;
}

// setRevealedCard
// Input: a vector pointer to a new hand
// Description: sets revealedCards to a new hand
// Output: none
void Player::setRevealedCards(Vector *revealedCards){
    this->revealedCards = revealedCards;
}

// setScore
// Input: an int to be added to score
// Description: adds the total points earned in the 
// round to the player's score
// Output: none
void Player::addScore(int score){
    this->score += score;
}

// setPuddingCount
// Input: an int to be added to puddingCount
// Description: adds the total puddings earned in the 
// round to the player's puddingCount
// Output: none
void Player::addPuddingCount(int puddingCount){
    this->puddingCount += puddingCount;
}