/*
   This program is designed to compete in the Taylor University Battelships AI contest
   This started as a class project which was Co-authored by Stefan Brandle, Jonathan Geisler, Andrew Blomenberg and Daniel Thomas
   Almost everything in here now has been written by Andrew Blomenberg
   The most recent edit session started in June 2017

   Sylok the Defiled is a reference to the game Destiny
 */

#include <iostream>
#include <cstdlib>

#include "conio.h"
#include "Sylok.h"

using namespace conio;
using namespace std;

/*
  Simple inner class Coord. Coord stores a row,column pair
  Also stores a priority for insertion into priority queues
  */
Coord::Coord(){
	priority=0;
}

Coord::Coord(int row, int col)
{
	this->row=row;
	this->col=col;
	priority=0;
}
bool Coord::equals(const Coord that) const
{
	return (this->row==that.row) && (this->col == that.col);
}
/*
   Overloads the == operator for Coord. FOR USE IN PQUEUE. 
   Only compares the priority.
 */
bool Coord::operator == (const Coord that) const
{
	return (this->priority==that.priority);
}
/*
   Overloads the < operator for Coord. FOR USE IN PQUEUE. 
   Only compares the priority.
   */
bool Coord::operator< (const Coord that) const
{
	return this->priority<that.priority;
}
/*
bool Coord::priorityLess(Coord first, Coord second)
{
	return first.priority<second.priority;
}
*/
bool Coord::inBounds() const
{
	return row<10 && col<10 && row>=0 && col>=0;
}

/*
   This constructor must have a parameter for the size of the board. 
   I have no idea what I would do if it was not 10 by 10 though.
   The constuctor is just here to initialize variables.
*/
Sylok::Sylok( int boardSize ):PlayerV2(10)
{
	if(boardSize !=10)
		throw "Sylok only works on boards of size 10";
	
	roundCount=0;
}
/*
   Destructor
   */
Sylok::~Sylok(){}

/*
  Private internal function that initializes board to be a 10X10 array of water.
 */
void Sylok::initializeBoard() {
    for(int row=0; row<10; row++) {
		for(int col=0; col<10; col++) {
		    this->board[row][col] = WATER;
		}	
    }
}


/*
   Returns Sylok's choice of shot in a Message
   Terrible Spaghetti, forgive me
*/
Message Sylok::getMove() 
{
	Coord finalShot; //where I will shoot
	
	//shotResult is updated in update()
	switch(shotResult)
	{
		case HIT:
			break;
		case KILL:
			hitPossIter("nearExistingShot");
			break;
		case MISS:
			break;
		case INVALID_SHOT:
			break;
	}
	hitScan();
	do {
		finalShot=hitPoss.top();
		hitPoss.pop();
	}while (!(finalShot.inBounds() &&  board[finalShot.row][finalShot.col] == WATER));

	prevShot=finalShot;
    Message result( SHOT, finalShot.row, finalShot.col, "Bang", None, 1 );
    return result;
}
/*
  Searches board for patterns of hits and calls appropriate helpers to enqueue high-probability points
  */
void Sylok::hitScan()
{
	
	if( shotResult==MISS)
		toPop--;
	while(toPop>0)
	{
		hitPoss.pop();
		toPop--;
	}

	for(int row=0; row<10; row++)
	{
		for(int col=0; col<10; col++)
		{
			if(board[row][col]==HIT)
			{
				Coord thisLoc(row,col);
				Coord neighbor(row,col);
				if(row>0 && board[row-1][col]==HIT)
					neighbor.row--;
				else if(col>0 && board[row][col-1]==HIT)
					neighbor.col--;
				else if(row<9 && board[row+1][col]==HIT)
					neighbor.row++;
				else if(col<9 && board[row][col+1]==HIT)
					neighbor.col++;
								
				if(!neighbor.equals(thisLoc) && linearShip(thisLoc,neighbor))
				{
					stdSink(thisLoc, neighbor);
					return;
				}
				//then it is either all alone or not on a linear ship

				toPop=insertSurrounding(thisLoc);
				
			}
		}
	}
}
/*
	returns true if the pair of coordinates given may be two hits on the same ship
	also modifies the coordinates given to represent the known ends of the ship
*/
bool Sylok::linearShip(Coord& coord1, Coord& coord2)
{
	bool horizontal; //true if the ship is horizontal
	horizontal=coord1.row==coord2.row;

	if(horizontal)
	{
		while(coord1.col<9 && board[coord1.row][coord1.col+1] == HIT)
			coord1.col++;
		while(coord2.col>0 && board[coord2.row][coord2.col-1] == HIT)
			coord2.col--;
		return((coord1.col==9 || board[coord1.row][coord1.col+1] != MISS) && 
		       (coord2.col==0 || board[coord2.row][coord2.col-1] != MISS));
	}else{
		while(coord1.row<9 && board[coord1.row+1][coord1.col] == HIT)
			coord1.row++;
		while(coord2.row>0 && board[coord2.row-1][coord2.col] == HIT)
			coord2.row--;
		return((coord1.row==9 || board[coord1.row+1][coord1.col] != MISS) && 
		       (coord2.row==0 || board[coord2.row-1][coord2.col] != MISS));
	}
}
/*
   Returns a priority that represents the likelyhood that a Coord is over a ship
   Uses the rand() function to give the Coordinates in hitPoss a random order
 */
int Sylok::hitPriority(string priorityType)
{
	int min, mod;
	if(priorityType=="kill")
	{
		min=400;
		mod=50;
	}
	else if(priorityType=="high")
	{
		min=300;
		mod=50;
	}
	else if(priorityType=="standard")
	{
		min=150;
		mod=100;
	}
	else if(priorityType=="low")
	{
		min=100;
		mod=50;
	}
	else
		throw new logic_error(priorityType+ " is not a valid priority");
	return (rand()%mod)+min;
}
/*
	Performs all the necessary checks before adding a point on hitPoss
	Checks for inBounds, over water, and if it is already in hitPoss
	returns true if the parameter is unique and valid
 */
bool Sylok::enqueuePoint(Coord insert)
{
	if(!insert.inBounds() || board[insert.row][insert.col]!=WATER)
		return false;

	bool duplicate=false;
	stack<Coord> swap;
	Coord top;
	while(!hitPoss.empty())
	{
		top=hitPoss.top();
		hitPoss.pop();
		if (insert.equals(top))
		{
			duplicate=true;
			if(top.priority<insert.priority)
				top.priority=insert.priority;
		}
		swap.push(top);

	}
	while(!swap.empty())
	{
		hitPoss.push(swap.top());
		swap.pop();
	}
	if(duplicate)
		return false;
	else
		hitPoss.push(insert);

	return true;

}
/*
   Helper to getMove that looks at the four tiles that surround a given point and then
   inserts them into hitPoss in a random order
   Returns the number of Coords Enqueued;
*/
int Sylok::insertSurrounding(Coord firstHit)
{
	Coord  insert;
	int enqueued=0;
	
	for(int neighbor=0; neighbor<4; neighbor++)
	{
		insert=firstHit;
		switch (neighbor)
		{
			case 0:
				insert.col++;
				break;
			case 1:
				insert.row++;
				break;
			case 2:
				insert.col--;
				break;
			case 3:
				insert.row--;
				break;
		}
		insert.priority=hitPriority("kill");
		if(enqueuePoint(insert))
			enqueued++;
	}
	return enqueued;
}

/*
 Adds high-priority Coords onto hitPoss which are on either end of the line created by end1 and end2.
 It assumes that both hits are on the same ship.
 Returns false if it's assumption was wrong
 */
bool Sylok::stdSink(Coord end1, Coord end2)
{
	bool horizontal; //true if the ship is horizontal
	bool enqueued=false; //true if a something was put on hitPoss

	horizontal=end1.row==end2.row;

	if(horizontal)
	{
		Coord right(end1.row, max(end2.col, end1.col)+1);
		Coord  left(end1.row, min(end2.col, end1.col)-1);
		right.priority=hitPriority("kill");
		left. priority=hitPriority("kill");
			
		enqueued=enqueuePoint(right);
		enqueued=enqueuePoint(left ) || enqueued;
	}
	else
	{
		Coord    top(max(end1.row, end2.row)+1, end1.col);
		Coord bottom(min(end1.row, end2.row)-1, end1.col);
		top.   priority=hitPriority("kill");
		bottom.priority=hitPriority("kill");
		
		enqueued=enqueuePoint(top   );
		enqueued=enqueuePoint(bottom) || enqueued;
	}

	return enqueued; 
}
/*
   Similar to a forEach. 
   Iterates through hitPoss and removes any coord for which the function parameter returns false.
   The function parameter may also modify each coord.
*/
void Sylok::hitPossIter(string fName)
{
	vector<Coord> swap;
	bool fResult=false;
	Coord top;
	while(!hitPoss.empty())
	{
		top= (Coord)hitPoss.top();
		hitPoss.pop();

		if(fName=="nearExistingShot")
			fResult=nearExistingShot(top);
		else if(fName=="duplicateCheck")
			fResult=duplicateCheck(top);
		else if(fName=="lowerPriority")
			fResult=lowerPriority(top);
		else
			throw new logic_error( fName+" not recognized");

		if(fResult)
		{
			swap.push_back(top);
		}
	}
	while(!swap.empty())
	{
		hitPoss.push(swap.back());
		swap.pop_back();
	}
}
/*
   One of the functions for hitPossIter
   returns true as long as the Coord is in bounds.
   Decreases the parameter's priority (150-100) if it is a regular shot near an existing shot
 */
bool Sylok::nearExistingShot(Coord& searchP) 
{
	if(!searchP.inBounds())
		return false;

	bool shotNearby=           searchP.col>0 && board[searchP.row][searchP.col-1] != WATER;
	shotNearby= shotNearby || (searchP.col<9 && board[searchP.row][searchP.col+1] != WATER);
	shotNearby= shotNearby || (searchP.row<9 && board[searchP.row+1][searchP.col] != WATER);
	shotNearby= shotNearby || (searchP.row>0 && board[searchP.row-1][searchP.col] != WATER);

	if (shotNearby)
	{
		searchP.priority=hitPriority("low");
	}
	return true;
}
/*
  One of the functions for hitPossIter
  Returns false if searchP is a duplicate
*/
bool Sylok::duplicateCheck(Coord searchP)
{
	vector<Coord> swap;

	while(!hitPoss.empty())
	{
		if(searchP.equals(hitPoss.top()))
			return false;
		swap.push_back(hitPoss.top());
		hitPoss.pop();
	}
	while(!swap.empty())
	{
		hitPoss.push(swap.back());
		swap.pop_back();
	}
	return true;
}
/*
 Changes any coordinate with a priority above 399 to a low priority location
 Always returns true
*/ 
bool Sylok::lowerPriority(Coord& searchP)
{
	if(searchP.priority>399)
		searchP.priority=hitPriority("low");
	return true;
}

/*
   Runs when a new game of battleship begins.
   Calls InitalizeBoard, Adds ship names to the list, and 
   sets up data structures for shooting and placing ships.
 */
void Sylok::newRound() {
   this->initializeBoard();
   this->roundCount++;

   //Better than Ship 1-Ship 10
   //shipNames is a stack
   shipNames.push("Foxtrot");
   shipNames.push("Echo");
   shipNames.push("Delta");
   shipNames.push("Charlie");
   shipNames.push("Bravo");
   shipNames.push("Alpha");


   //for placeOutside
   usedStarts.clear();
   
   //effectively empties startCoords
   startCoords.resize(0);
   startCoords.resize(3);

   fillStartCoords();
   
   //for shooting 
   this->shotResult=INVALID_SHOT;
   this->toPop=0;
   
   while(true)
   {
	 if (hitPoss.empty())
	   break;
	 hitPoss.pop();
   }
   generateShootCoords();
}
/*
   Helper for newRound that fills hitPoss with a set of coordinates that form an inescapable pattern.
   Each Coordinate inserted initially has a priority between 150 and 250. 
   */
void Sylok::generateShootCoords()
{
	int colStart=2; //Starting Column. Could be 0,1,or 2, but past me made it 2
   Coord toPush;
   int priority;

   while(!hitPoss.empty())
	   hitPoss.pop();
   
   for (int row=0; row<10; row++)
   {
	 for(int col=colStart; col<10;col+=3)
	 {
		priority=hitPriority("standard");
		toPush.row=row;
		toPush.col=col;
		toPush.priority=priority;
		hitPoss.push(toPush);
	}
	colStart--;
	if (colStart==-1)
		colStart=2;
   }
}
/*
   Helper for placeOutside called in newRound
   Adds the four corners and two other points to each PQ in startCoords
   Each ship will be placed unless two can share a side
   */
void Sylok::fillStartCoords()
{
	enqueueCoord(0,0,10+rand()%5);
	enqueueCoord(0,9,10+rand()%5);
	enqueueCoord(9,0,10+rand()%5);
	enqueueCoord(9,9,10+rand()%5);
	enqueueCoord(2,2,1);
	enqueueCoord(7,7,1);
	
}
/*
   Helper for fillStartCoords
   */
void Sylok::enqueueCoord(int row, int col, int priority)
{
   
   Coord toPush(row, col);
   toPush.priority=priority;
   for(int stackIdx=0; stackIdx<3; stackIdx++)
   {
	   startCoords.at(stackIdx).push(toPush);
   }
}

/*
   PlaceShip decides where to put a ship of given length.
   It calls different placing functions based on which one works well. 
   Currently, it only calls placeOutside because that is the only function I have working

   The parameters returned via the message are:
   1. the operation: must be PLACE_SHIP 
   2. ship top row value
   3. ship top col value
   4. a string for the ship name
   5. direction Horizontal/Vertical (see defines.h)
   6. ship length (should match the length passed to placeShip)
 */

Message Sylok::placeShip(int length) {
    return placeOutside(length);
}
Message Sylok::placeOutside(int length) {

   Message outMsg(PLACE_SHIP);

   //Check the stacks for invalid cooridnates
   removeInvalidCoords();
   
   //place a ship clockwise
   Coord tempPair=startCoords.at(length-3).top();
   startCoords.at(length-3).pop();

   usedStarts.push_back(tempPair); 
   outMsg=placeClockwise(tempPair.row,tempPair.col,length); //modifies tempPair as well

   if(length==3 && tempPair.priority<20) 
    //check the priority because the highest priority do not have space behind them
   {
	   // a length 3 ship can (and should) share a side with either a 3 or a 4 ship
	   tempPair.priority=20;
	   startCoords.at(0).push(tempPair);
	   startCoords.at(1).push(tempPair);
   }
   if(length==4 && tempPair.priority<20)
   {
	   // a length 4 ship can (and should) share a side with a 3 ship
	   tempPair.priority=20;
	   startCoords.at(0).push(tempPair);
   }
   return outMsg;

}
/*
   Helper for PlaceOutside that removes any Coords that have been used already from the StartCoords
   */
void Sylok::removeInvalidCoords()
{
   stack<Coord> tempStack;
   bool goodCoord;

   for(int stk=0; stk<3; stk++)
   {
	 while(!startCoords.at(stk).empty())
	 {
	    goodCoord=true;
	    
		if(!usedStarts.empty())
		{
			// bad Coord if it has already been used
			for(unsigned int idx=0; idx<usedStarts.size(); idx++)
			{
				if (usedStarts.at(idx)== startCoords.at(stk).top())
				{
					goodCoord=false;
				}
			}
		}
		if(!(startCoords.at(stk).top().inBounds()))
			goodCoord=false;

	    if(goodCoord)
		  tempStack.push(startCoords.at(stk).top());
	    
	    startCoords.at(stk).pop();
	    
	 }
	 //put the good coordinates back into the stack
	 while(!tempStack.empty())
	 {
	    startCoords.at(stk).push(tempStack.top());
	    tempStack.pop();
	 }
   }

}
/*
   This function takes a row, column, and length and returns a message that makes a legal move
   it also MODIFIES ROW AND COLUMN to be off the end of the ship that was placed
   */
Message Sylok::placeClockwise(int& row, int& col, int length)
{
   vector< vector<string> > dirGrids(3);


   dirGrids.at(0).resize(10);
   dirGrids.at(1).resize(10);
   dirGrids.at(2).resize(10);

   dirGrids.at(0).at(0)="rrrrrrrrdd";
   dirGrids.at(0).at(1)="rrrrrrrddd";
   dirGrids.at(0).at(2)="urrrrrdddd";
   dirGrids.at(0).at(3)="uurrrddddd";
   dirGrids.at(0).at(4)="uuurrddddd";
   dirGrids.at(0).at(5)="uuuuullddd";
   dirGrids.at(0).at(6)="uuuuullldd";
   dirGrids.at(0).at(7)="uuuullllld";
   dirGrids.at(0).at(8)="uuulllllll";
   dirGrids.at(0).at(9)="uullllllll";
   
   dirGrids.at(1).at(0)="rrrrrrrddd";
   dirGrids.at(1).at(1)="rrrrrrdddd";
   dirGrids.at(1).at(2)="rrrrrddddd";
   dirGrids.at(1).at(3)="urrrdddddd";
   dirGrids.at(1).at(4)="uurrdddddd";
   dirGrids.at(1).at(5)="uuuuullddd";
   dirGrids.at(1).at(6)="uuuuuullld";
   dirGrids.at(1).at(7)="uuuuulllll";
   dirGrids.at(1).at(8)="uuuullllll";
   dirGrids.at(1).at(9)="uuulllllll";
   
   dirGrids.at(2).at(0)="rrrrrrdddd";
   dirGrids.at(2).at(1)="rrrrrrdddd";
   dirGrids.at(2).at(2)="rrrrrddddd";
   dirGrids.at(2).at(3)="rrrrrddddd";
   dirGrids.at(2).at(4)="uurrrddddd";
   dirGrids.at(2).at(5)="uuuuullldd";
   dirGrids.at(2).at(6)="uuuuulllll";
   dirGrids.at(2).at(7)="uuuuulllll";
   dirGrids.at(2).at(8)="uuuullllll";
   dirGrids.at(2).at(9)="uuuullllll";

   /*
   dirGrid[0]={'r','r','r','r','r','r','r','r','r','d'};
   dirGrid[1]={'u','r','r','r','r','r','r','r','d','d'};
   dirGrid[2]={'u','u','r','r','r','r','r','d','d','d'};
   dirGrid[3]={'u','u','u','r','r','r','d','d','d','d'};
   dirGrid[4]={'u','u','u','u','r','d','d','d','d','d'};
   dirGrid[5]={'u','u','u','u','u','l','d','d','d','d'};
   dirGrid[6]={'u','u','u','u','l','l','l','d','d','d'};
   dirGrid[7]={'u','u','u','l','l','l','l','l','d','d'};
   dirGrid[8]={'u','u','l','l','l','l','l','l','l','d'};
   dirGrid[9]={'u','l','l','l','l','l','l','l','l','l'};
   
    */
   Direction msgDir=Horizontal;
   int msgRow=row;
   int msgCol=col;

   char dir= dirGrids.at(length-3).at(row)[col];

   switch(dir)
   {
	 case'l':
	    msgDir=Horizontal;
	    msgCol-=(length-1);
	    col-=(length+1);
	    break;
	 case'r':
	    msgDir=Horizontal;
	    col+=length+1;
	    break;
	 case 'u':
	    msgDir=Vertical;
	    row-=length+1;
	    msgRow-=(length-1);
	    break;
	 case 'd':
	    msgDir=Vertical;
	    row+=(length+1);
	    break;
   }

   Message response(PLACE_SHIP,msgRow, msgCol,shipNames.top(),msgDir,length);
   shipNames.pop();

   return response;
}
/*
   Gets information from the game as a message.
   This function will eventually be a way to gather data for the learning algorithms
 */
void Sylok::update(Message msg) {
    switch(msg.getMessageType()) {
	case HIT:
	case KILL:
	case MISS:
	    board[msg.getRow()][msg.getCol()] = msg.getMessageType();
		shotResult=msg.getMessageType();
	    break;
	case WIN:
	    break;
	case LOSE:
	    break;
	case TIE:
	    break;
	case OPPONENT_SHOT:
	    break;
    }
}

