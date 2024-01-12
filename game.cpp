/*
 * game.cpp
 * CS11 Sushi Go
 */

#include "game.h"
#include <iostream>

using namespace std;

//constructor
Game::Game(string filename, string play_chopsticks)
{
    //include chopsticks or not
    if(play_chopsticks == "true"){
        this->play_chopsticks = true;
    }else{
        this->play_chopsticks = false;
    }  

    ifstream infile(filename);
    if (infile.fail()) {
        cerr << "Failed to open deck file: " << filename << endl;
        exit(EXIT_FAILURE);
    }

    // Read in all the cards from the file, making a card for each one and
    // storing it in the deck vector.
    string cardName, misc;
    int cardNum;
    infile >> misc >> misc;
    while(!infile.eof()) 
    {
        if (infile.fail()) {
            cerr << "Failed to read from file: " << filename << endl;
            exit(EXIT_FAILURE);
        }

        //checks if chopsticks is being used
        infile >> cardName;
        if(!this->play_chopsticks){
            if(cardName == "Chopsticks"){
                infile >> cardName;
            }
        }

        if(infile.eof()){
            break;
        }

        //reads in makiCount
        if(cardName == "Maki"){
            infile >> cardNum;
        } else{
            cardNum = 0;
        }
        //creates cards to be places in the vector
        Card *newCard = new Card(cardName, cardNum);
        deck.insert(0, newCard);
    }
}

//main game loop
void Game::playGame(){
    int card_index;
    string play_on;

    //play three rounds
    for(int round = 0; round < ROUNDS; round++){
        //Deal 9 cards to each hand
        for(int i = 0; i < PLAYER_COUNT; i++){
            for(int j = 0; j < CARD_HAND * 3; j += 3){
                hand[i].push_back(deck.at(j + i + (round * 27)));
            }
        }
        //Assign hands to players
        for(int i = 0; i < PLAYER_COUNT; i++){
            players[i].setPassingHand(&hand[i]);
            players[i].setRevealedCards(&keptCards[i]);
        }

        //select and pass all 27 cards
        Card *tempCard[PLAYER_COUNT];//to save cards before they are deleted
        for(int card = 0; card < CARD_HAND; card++){
            for(int player = 0; player < PLAYER_COUNT; player++){
                board.drawBoard(players, player);
                cout << " Player " << player + 1 << ", select a card: ";
                cin >> card_index;
                while(card_index < 1 || card_index > CARD_HAND - card){
                    cout << "     Please enter a valid number between 1 and " 
                        << CARD_HAND - card << ": ";
                    cin >> card_index;
                }

                //set aside selected cards and erase from hand
                card_index--;
                tempCard[player] = players[player].getPassingHand()
                    ->at(card_index);
                players[player].getPassingHand()->erase(card_index);
            }
            //reveal selected cards
            for(int i = 0; i < PLAYER_COUNT; i++){
                keptCards[i].push_back(tempCard[i]);
            }

            //pass hands to the right
            Vector *tempHand = players[0].getPassingHand();
            players[0].setPassingHand(players[2].getPassingHand());
            players[2].setPassingHand(players[1].getPassingHand());
            players[1].setPassingHand(tempHand);
        }
        //Update Scores
        //Variables to keep track of score
        int maki[PLAYER_COUNT] = {0, 0, 0};
        int dumpling[PLAYER_COUNT] = {0, 0, 0};
        int rScore[PLAYER_COUNT] = {0, 0, 0};
        int rPudding[PLAYER_COUNT] = {0, 0, 0};

        //Functions to determine points
        determineScore(maki, dumpling, rScore, rPudding);
        determineMakiScore(maki, rScore);
        determineDumplingScore(dumpling, rScore);

        //Adds tallied scores and puddings into the players'
        //score and puddingCount variables
        for(int i = 0; i < PLAYER_COUNT; i++){
            players[i].addScore(rScore[i]);
            players[i].addPuddingCount(rPudding[i]);
        }

        //Draw player scores and ask to continue to next round
        board.drawScore(players);    
        if(round < 2){
            cout << " End of round! Ready for Round "
                 << round + 2 << " ? (y/n): ";
            cin >> play_on;
            if(play_on != "y"){
                break;
            }
        }

        //Reset for next round
        for(int i = 0; i < CARD_HAND; i++){
            for(int j = 0; j < PLAYER_COUNT; j++){
                keptCards[j].erase(0);
            }
        }
    }

    //Score pudding
    int score[PLAYER_COUNT] = {0, 0, 0};
    determinePuddingScore(players[0].getPuddingCount(), 
        players[1].getPuddingCount(), 
        players[2].getPuddingCount(), score);
        
    for(int i = 0; i < PLAYER_COUNT; i++){
        players[i].addScore(score[i]);
    }

    //Determine winner
    int winner;
    determineWinner(players[0].getScore(), 
        players[1].getScore(), 
        players[2].getScore(), &winner);

    board.drawWinner(players, winner);
}

// determineScore
// Input: multiple int arrays to score and pudding
// Description: adds score to coresponding rScore array for 
// everything but maki and dumpling
// Output: none
void Game::determineScore(int *maki, int *dumpling, int *rScore, int *rPudding){
    string sushi;
    for(int i = 0; i < PLAYER_COUNT; i++){
        int tempura = 0;
        int sashimi = 0;
        bool wasabi[4] = {false, false, false, false};
        for(int j = 0; j < CARD_HAND; j++){
            sushi = players[i].getRevealedCards()->at(j)
                ->getSushiType();
            if(sushi == "Maki"){
                maki[i] += players[i].getRevealedCards()
                    ->at(j)->getMakiCount();
            } else if(sushi == "Tempura"){
                tempura++;
                if(tempura == 2){
                    tempura = 0;
                    rScore[i] += 5;
                }
            } else if(sushi == "Sashimi"){
                sashimi++;
                if(sashimi == 3){
                    sashimi = 0;
                    rScore[i] += 10;
                }
            } else if(sushi == "Dumpling"){
                dumpling[i]++;
            } else if(sushi == "Wasabi"){
                for(int k = 0; k < 4; k++){
                    if(!wasabi[k]){
                        wasabi[k] = true;
                        break;
                    }
                }
            } else if(sushi == "Egg-Nigiri"){
                for(int k = 0; k < 4; k++){
                    if(wasabi[k]){
                        rScore[i] += 2;
                        wasabi[k] = false;
                        break;
                    }
                }
                rScore[i] += 1;
            } else if(sushi == "Salmon-Nigiri"){
                for(int k = 0; k < 4; k++){
                    if(wasabi[k]){
                        rScore[i] += 4;
                        wasabi[k] = false;
                        break;
                    }
                }
                rScore[i] += 2;
            } else if(sushi == "Squid-Nigiri"){
                for(int k = 0; k < 4; k++){
                    if(wasabi[k]){
                        rScore[i] += 6;
                        wasabi[k] = false;
                        break;
                    }
                }
                rScore[i] += 3;
            } else if(sushi == "Pudding"){
                rPudding[i] += 1;
            }
        }
    }
}

// determineMakiScore
// Input: maki array and score array
// Description: determines maki scoring for each round
// Output: none
void Game::determineMakiScore(int *maki, int *rScore){
    if(maki[0] > maki[1]){
        if(maki[0] > maki[2]){
            rScore[0] += 6; 
            if(maki[1] > maki[2]){
                rScore[1] += 3;
            } else if(maki[2] > maki[1]){
                rScore[2] += 3;
            } else {
                rScore[1] += 1;
                rScore[2] += 1;
            }
        } else if(maki[2] > maki[0]){
            rScore[2] += 6;
            rScore[0] += 3;
        } else {
            rScore[0] += 3;
            rScore[2] += 3;
        }
    } else if(maki[1] > maki[0]){
        if(maki[1] > maki[2]){
            rScore[1] += 6; 
            if(maki[0] > maki[2]){
                rScore[0] += 3;
            } else if(maki[2] > maki[0]){
                rScore[2] += 3;
            } else {
                rScore[0] += 1;
                rScore[2] += 1;
            }
        } else if(maki[2] > maki[1]){
            rScore[2] += 6;
            rScore[1] += 3;
        } else {
            rScore[1] += 3;
            rScore[2] += 3;
        }
    } else{
        if(maki[2] > maki[0]){
            rScore[2] += 6;
            rScore[0] += 1;
            rScore[1] += 1;
        } else if(maki[2] < maki[0]){
            rScore[0] += 3;
            rScore[1] += 3;
        } else{
            rScore[0] += 2;
            rScore[1] += 2;
            rScore[2] += 2;
        }
    }
}

// determineDumplingScore
// Input: dumpling array and score array
// Description: determines dumpling scoring for each round
// Output: none
void Game::determineDumplingScore(int *dumpling, int *rScore){
    for(int i = 0; i < PLAYER_COUNT; i++){
        if(dumpling[i] == 1){
            rScore[i] += 1;
        } else if(dumpling[i] == 2){
            rScore[i] += 3;
        } else if(dumpling[i] == 3){
            rScore[i] += 6;
        } else if(dumpling[i] == 4){
            rScore[i] += 10;
        } else if(dumpling[i] > 4){
            rScore[i] += 15;
        }
    }
}

// determinePuddingScore
// Input: player pudding amounts and score array
// Description: determines pudding scoring at end of game
// Output: none
void Game::determinePuddingScore(int player1, int player2, int player3, int *score){
    if(player1 > player2){
        if(player1 > player3){
            score[0] += 6; 
            if(player2 > player3){
                score[2] -= 6;
            } else if(player3 > player2){
                score[1] -= 6;
            } else {
                score[1] -= 3;
                score[2] -= 3;
            }
        } else if(player3 > player1){
            score[2] += 6;
            score[1] -= 6;
        } else {
            score[0] += 3;
            score[2] += 3;
            score[1] -= 6;
        }
    } else if(player2 > player1){
        if(player2 > player3){
            score[1] += 6; 
            if(player1 > player3){
                score[2] -= 6;
            } else if(player3 > player1){
                score[0] -= 6;
            } else {
                score[0] -= 3;
                score[2] -= 3;
            }
        } else if(player3 > player2){
            score[2] += 6;
            score[0] -= 6;
        } else {
            score[1] += 3;
            score[2] += 3;
            score[0] -= 6;
        }
    } else{
        if(player3 > player1){
            score[2] += 6;
            score[0] -= 3;
            score[1] -= 3;
        } else if(player3 < player1){
            score[0] += 3;
            score[1] += 3;
            score[2] -= 6;
        }
    }
}

// determineWinner
// Input: player scores and pointer to winner int
// Description: determines the winner of the game
// Output: none
void Game::determineWinner(int player1, int player2, int player3, int *winner){
    if(player1 > player2){
        if(player1 > player3){
            *winner = 0;
        } else if(player3 > player1){
            *winner = 2;
        } else{
            if(players[0].getPuddingCount() > players[2].getPuddingCount()){
                *winner = 0;
            } else if(players[0].getPuddingCount() < players[2].getPuddingCount()){
                *winner = 2;
            } else{
                *winner = -1;
            }
        } 
    } else if(player2 > player1){
        if(player2 > player3){
            *winner = 1;
        } else if(player3 > player2){
            *winner = 2;
        } else{
            if(players[1].getPuddingCount() > players[2].getPuddingCount()){
                *winner = 1;
            } else if(players[1].getPuddingCount() < players[2].getPuddingCount()){
                *winner = 2;
            } else{
                *winner = -1;
            }
        }
    } else{
        if(player3 > player1){
            *winner = 2;
        } else if(player1 > player3){
            if(players[0].getPuddingCount() > players[1].getPuddingCount()){
                *winner = 0;
            } else if(players[0].getPuddingCount() < players[1].getPuddingCount()){
                *winner = 1;
            } else{
                *winner = -1;
            }
        } else{
            if(players[0].getPuddingCount() > players[1].getPuddingCount()){
                if(players[0].getPuddingCount() > players[2].getPuddingCount()){
                    *winner = 0;
                } else if(players[0].getPuddingCount() < players[2].getPuddingCount()){
                    *winner = 2;
                } else{
                    *winner = -1;
                }
            } else if(players[0].getPuddingCount() < players[1].getPuddingCount()){
                if(players[1].getPuddingCount() > players[2].getPuddingCount()){
                    *winner = 1;
                } else if(players[1].getPuddingCount() < players[2].getPuddingCount()){
                    *winner = 2;
                } else{
                    *winner = -1;
                }
            } else{
                if(players[0].getPuddingCount() > players[2].getPuddingCount()){
                    *winner = -1;
                } else if(players[0].getPuddingCount() < players[2].getPuddingCount()){
                    *winner = 2;
                } else{
                    *winner = -1;
                }
            }
        }
    }
}

//destructor
Game::~Game(){
    for(int i = 0; i < deck.size(); i++){
        delete deck.at(i);
    }
}
