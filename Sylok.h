
/**
 * @author Stefan Brandle, Jonathan Geisler
 * @date September, 2004
 *
 * Please type in your meme[s] here: Andrew Blomenberg and Daniel Thomas
 *
 */

#ifndef Sylok_H		// Double inclusion protection
#define Sylok_H

using namespace std;

#include "PlayerV2.h"
#include "Message.h"
#include "defines.h"
#include <vector>
#include <stack>
#include <queue>
#include <string>

class Coord //Simple inner class to store a row, column pair
{
	public:
	int row,col,priority;
	Coord(); 
	Coord(int row, int col);
	bool equals(Coord that) const;
	bool operator== (const Coord that) const;
//	bool priorityLess(Coord,Coord);
	bool operator< (const Coord that) const;
	bool inBounds() const;
};
// Sylok inherits from/extends PlayerV2
class Sylok: public PlayerV2 {

	public:
	Sylok( int boardSize );
	~Sylok();
	void newRound();
	Message placeShip(int length);
	Message getMove();
	void update(Message msg);

	private:
	void initializeBoard(); //helper for newRound
	void hitScan(); //helper for getMove
	bool linearShip(Coord&, Coord&); //helper for hitScan
	int hitPriority(string); //helper for getMove helpers (woah, meta)
	bool enqueuePoint(Coord); //helper for getMove helpers (double meta)
	int insertSurrounding(Coord firstHit); //helper for getMove
	bool stdSink(Coord, Coord);//helper for getMove
	void hitPossIter(string fName); //helper for getMove
	bool lowerPriority(Coord&); //Urinary function for hitPossIter
	bool nearExistingShot(Coord& ); //Urinary function for hitPossIter
	bool duplicateCheck(Coord);  //Urinary function for hitPossIter
	void fillStartCoords(); //helper for newRound and placeOutside
	void enqueueCoord(int,int,int);//helper for fillStartCoord
	void removeInvalidCoords(); //helper for PlaceOutside
	void generateShootCoords(); //Helper for newRound

	Message placeClockwise(int& row, int& col, int length); //returns a message to place a ship clockwise
	Message placeOutside(int length); //functions as one potential placeShip method
	Message placeRandom(int length); //placeholder for future placement
	
	char board[MAX_BOARD_SIZE][MAX_BOARD_SIZE];
		
	//Variables for shooting
	//vector<vector<Coord> > shootPairs; //stores row,col pairs that can be used to shoot 
	char shotResult; //stores the result of the last shot
	int roundCount; //counts the number of rounds in the game
	int toPop; //stores the number of items to be popped off hitPoss
	Coord prevShot; //stores the location of the last shot 
	priority_queue<Coord> hitPoss; //stores all the shooting possibilities
	Coord firstHitLoc; //Stores the location of the first hit on the ship
	
	//Variables for placeOutside
	vector< priority_queue<Coord> > startCoords; //each PQ holds starting points for a size of ship
	vector< Coord > usedStarts;

	//General purpose ship placement
	stack<string> shipNames;

};
#endif
