/*
 * game.h
 * COMP11 Sushi Go
 */

#ifndef GAME_H
#define GAME_H
#include <iostream>
#include <fstream>
#include <string>
#include "termfuncs.h"
#include "board.h"
#include "vector.h"
#include "player.h"
using namespace std;

class Game{
    public:
        //constructor/destructor
        Game(string filename, string play_chopsticks);
        ~Game();

        //main gameplay
        void playGame();

    private:
        //constants
        static const int PLAYER_COUNT = 3;
        static const int ROUNDS = 3;
        static const int CARD_HAND = 9;

        //score tallies
        void determineScore(int *maki, int *dumpling, int *rScore, int *rPudding);
        void determineMakiScore(int *maki, int *rScore);
        void determineDumplingScore(int *dumpling, int *rScore);
        void determinePuddingScore(int player1, int player2, int player3, int *score);
        void determineWinner(int player1, int player2, int player3, int *winner);

        //chopsticks activated true/false
        bool play_chopsticks;

        //card deck
        Vector deck;

        //card hands
        Vector hand[PLAYER_COUNT];

        //player chosen cards
        Vector keptCards[PLAYER_COUNT];

        //game objects
        Board board;
        Player players[PLAYER_COUNT];
};

#endif
