/**
 * @brief An AI to play in the Battleships AI contest
 * @file Sylok.cpp
 * @author Stefan Brandle, Jonathan Geisler, Andrew Blomenberg
 * @date June 2016
 *
 * This Battleships AI channels the might of SYLOK THE DEFILED to crush it's opponents.
 */

#include <iostream>
#include <cstdio>
#include <vector>
#include <cstdlib>
#include <string>

#include "conio.h"
#include "Sylok.h"
#include <stack>

using namespace conio;
using namespace std;

/**
 * @brief Constructor that initializes any inter-round data structures.
 * @param boardSize Indication of the size of the board that is in use.
 *
 * The constructor runs when the AI is instantiated (the object gets created)
 * and is responsible for initializing everything that needs to be initialized
 * before any of the rounds happen. The constructor does not get called 
 * before rounds; newRound() gets called before every round.
 */
Sylok::Sylok( int boardSize ) 
	:PlayerV2(boardSize)
{
    // Could do any initialization of inter-round data structures here.

	this->boardSize=boardSize;
	roundCount=0;
	shotResult=INVALID_SHOT;
	firstHitLoc.resize(2);
	prevShot.resize(2);
	shootPairs.resize(2);
}

/**
 * @brief Destructor placeholder.
 * If your code does anything that requires cleanup when the object is
 * destroyed, do it here in the destructor.
 */
Sylok::~Sylok( ) {}

/*
 * Private internal function that initializes a MAX_BOARD_SIZE 2D array of char to water.
 */
void Sylok::initializeBoard() {
    for(int row=0; row<boardSize; row++) {
	for(int col=0; col<boardSize; col++) {
	    this->board[row][col] = WATER;
	}	
    }
}


/**
 * @brief Specifies the AI's shot choice and returns the information to the caller.
 * @return Message The most important parts of the returned message are 
 * the row and column values. 
 *
 * See the Message class documentation for more information on the 
 * Message constructor.
 */
Message Sylok::getMove() 
{
	int row=0;
	int col=0;
	vector<int> sub(2);
	vector<vector<int> > stackRep;
	int repIdx=0;


	switch(shotResult)
	{
		case HIT:
			if (!hasHit)
			{
				firstHit=true;
				firstHitLoc=prevShot;
			}
			else
			{
				firstHit=false;
			}

			hasHit=true;

			if (firstHit)
			{
				int start=rand()%4;
				vector<int> insert(2);
				
				for(int idx=0; idx<4; idx++)
				{
					switch (start)
					{
						case 0:
							insert.at(0)=prevShot.at(0);
							insert.at(1)=prevShot.at(1)+1;
							break;
						case 1:
							insert.at(0)=prevShot.at(0)+1;
							insert.at(1)=prevShot.at(1);
							break;
						case 2:
							insert.at(0)=prevShot.at(0);
							insert.at(1)=prevShot.at(1)-1;
							break;
						case 3:
							insert.at(0)=prevShot.at(0)-1;
							insert.at(1)=prevShot.at(1);
							break;
					}
					hitPoss.push(insert);
					
					start++;
					if(start>3)
						start=0;
				}
			}
			else
			{
				bool manCol=false;
				bool manRow=false;
				short direction=0;
				
				if (prevShot.at(0) != firstHitLoc.at(0))
				{
					manRow=true;
					if (prevShot.at(0) < firstHitLoc.at(0))
						direction=-1;
					else
						direction=1;
				}
				else
				{
					manCol=true;
					if(prevShot.at(1) < firstHitLoc.at(1))
						direction=-1;
					else
						direction=1;
				}
				vector<int> insert(2);
				
				insert.at(0)=firstHitLoc.at(0)+(!manRow);
				insert.at(1)=firstHitLoc.at(1)+(!manCol);

				hitPoss.push(insert);

				insert.at(0)=firstHitLoc.at(1)-(!manCol);
				insert.at(1)=firstHitLoc.at(1)-(!manRow);

				hitPoss.push(insert);
				
				insert.at(0)=firstHitLoc.at(0)-(direction*manRow);
				insert.at(1)=firstHitLoc.at(1)-(direction*manCol);
				
				hitPoss.push(insert);
				
				insert.at(0)=prevShot.at(0)+(direction*manRow);
				insert.at(1)=prevShot.at(1)+(direction*manCol);

				hitPoss.push(insert);


			}
			while(!hitPoss.empty())
			{
				sub=hitPoss.top();
				hitPoss.pop();
				
				if(!(sub.at(0)>=boardSize or sub.at(1)>=boardSize or sub.at(0)<0 or sub.at(1)<0 ) && !(board[sub.at(0)][sub.at(1)] != WATER))
				{
					bool in=false;
					for(unsigned int iter=0; iter<stackRep.size(); iter++)
					{
						in=stackRep.at(iter)==sub;
						
						if(in)
							break;
					}
					if (!in)
						stackRep.push_back(sub);
				}

			}

			for(repIdx=stackRep.size()-1; repIdx>=0; repIdx--)
			{
				hitPoss.push(stackRep.at(repIdx));
			}

			//TODO make an intelligent response so this never happens
			if(hitPoss.empty())
			{
				shotResult=INVALID_SHOT;
				return getMove();
			}

			sub=hitPoss.top();
			hitPoss.pop();
			row=sub.at(0);
			col=sub.at(1);

			break;
		case KILL:
			hasHit=false;
			firstHit=false;
		case MISS:
			if (hasHit && !hitPoss.empty())
			{
				//TODO see above comment
				if(hitPoss.empty())
				{
					shotResult=INVALID_SHOT;
					return getMove();
				}
				sub=hitPoss.top();
				hitPoss.pop();

				row=sub.at(0);
				col=sub.at(1);
				break;
			}

			for(int r=0; r<boardSize; r++)
			{
				for (int c=0; c<boardSize; c++)
				{
					if(board[r][c]==HIT)
					{
						shotResult=HIT;
						prevShot.at(0)=r;
						prevShot.at(1)=c;
						return getMove();
					}
				}
			}

		case INVALID_SHOT:

			int swi=0;
			int rnd=0;
			unsigned int index=0;
			int r,c;

			for(; swi<2; swi++)
			{
				index=0;
				while(index<shootPairs.at(swi).size())
				{
					r=shootPairs.at(swi).at(index).at(0);
					c=shootPairs.at(swi).at(index).at(1);

					if(swi==0 && (board[r][c-1] != WATER || board[r][c+1] != WATER || board[r+1][c] != WATER || board[r-1][c] != WATER))
					{
							shootPairs.at(1).push_back(shootPairs.at(0).at(index));
							shootPairs.at(0).erase(shootPairs.at(swi).begin()+index);
					}
					else if(board[r][c] != WATER)
						shootPairs.at(swi).erase(shootPairs.at(swi).begin()+index);
					else
						index++;
				}
			}
			
			if (shootPairs.at(0).empty())
				swi=1;
			else
				swi=0;

			if (shootPairs.at(1).empty())
			{
				if(!hitPoss.empty())
				{
					sub=hitPoss.top();
					row=sub.at(0);
					col=sub.at(1);
				}
				else
				{
					row=0;
					col=0;
				}
				
			}
			else
			{
				rnd=rand() % shootPairs.at(swi).size();
				row=shootPairs.at(swi).at(rnd).at(0);
				col=shootPairs.at(swi).at(rnd).at(1);
				shootPairs.at(swi).erase(shootPairs.at(swi).begin()+rnd);
			}

			break;
	}
	prevShot.at(0)=row;
	prevShot.at(1)=col;
    Message result( SHOT, row, col, "Bang", None, 1 );
    return result;
}

/**
 * @brief Tells the AI that a new round is beginning.
 * The AI show reinitialize any intra-round data structures.
 */
void Sylok::newRound() {
   this->initializeBoard();
   this->roundCount++;
   
   //for placeOutside
   for(int stk=0; stk<3; stk++)
   {
	 while(true)
	 {
	    if (startStacks.at(stk).empty())
		  break;
	    startStacks.at(stk).pop();
	 }
   }

   //add the corners to the three stacks in a random order
   int select = rand() % 4;
   vector<int> pair(2);
   for(int count=0; count<4; count++)
   {
	 switch(select)
	 {
	    case 0:
		  pair.at(0)=0;
		  pair.at(1)=0;
		  break;
	    case 1:
		  pair.at(0)=0;
		  pair.at(1)=9;
		  break;
	    case 2:
		  pair.at(0)=9;
		  pair.at(1)=0;
		  break;
	    case 3:
		  pair.at(0)=9;
		  pair.at(1)=9;
		  break;
	 }
	 
	 if (select==3)
	    select=0;
	 else
	    select++;

	 startStacks.at(0).push(pair);
	 startStacks.at(1).push(pair);
	 startStacks.at(2).push(pair);
   }

   //for shooting 
   this->hasHit=false;
   this->firstHit=false;
   
   while(true)
   {
	 if (hitPoss.empty())
	   break;
	 hitPoss.pop();
   }
   
   int colStart=2;
   int swi=0;
   
   shootPairs.at(0).resize(0);
   shootPairs.at(1).resize(0);
   
   for (int row=0; row<boardSize; row++)
   {
	 for(int col=colStart; col<boardSize;col+=3)
	 {
	    if(swi==1)
			swi=0;
		
	    if(row==9 or row==0 or col==0 or col==9)
			swi=1;
		
		pair.at(0)=row;
		pair.at(1)=col;
		shootPairs.at(swi).push_back(pair);
	}
	colStart--;
	if (colStart==-1)
		colStart=2;
   }
}

/**
 * @brief Gets the AI's ship placement choice. This is then returned to the caller.
 * @param length The length of the ship to be placed.
 * @return Message The most important parts of the returned message are 
 * the direction, row, and column values. 
 *
 * The parameters returned via the message are:
 * 1. the operation: must be PLACE_SHIP 
 * 2. ship top row value
 * 3. ship top col value
 * 4. a string for the ship name
 * 5. direction Horizontal/Vertical (see defines.h)
 * 6. ship length (should match the length passed to placeShip)
 */

Message Sylok::placeShip(int length) {
    return placeOutside(length);
}
Message Sylok::placeOutside(int length) {
   char shipName[10];
   int outRow=0;
   int outCol=0;
   Direction outDir=Horizontal;

   // Create ship names each time called: Ship0, Ship1, Ship2, ...
   snprintf(shipName, sizeof shipName, "Ship%d", numShipsPlaced);

   //Check the stacks for invalid cooridnates
   stack< vector<int> > tempStack;
   bool goodPair;

   for(int stk=0; stk<3; stk++)
   {
	 while(!(startStacks.at(stk).empty() or usedStarts.empty()))
	 {
	    goodPair=true;
	    
	    for(unsigned int idx=0; idx<usedStarts.size(); idx++)
	    {
		  if (startStacks.at(stk).top()==usedStarts.at(idx))
			goodPair=false;
	    }
	    
	    if(goodPair)
	    {
		  tempStack.push(startStacks.at(stk).top());
		  startStacks.at(stk).pop();
	    }
	 }
	 while(!tempStack.empty())
	 {
	    startStacks.at(stk).push(tempStack.top());
	    tempStack.pop();
	 }
   }


   Message response(PLACE_SHIP, outRow, outCol,shipName,outDir,length);
   return response;
}
//Message Sylok::p
/**
 * @brief Updates the AI with the results of its shots and where the opponent is shooting.
 * @param msg Message specifying what happened + row/col as appropriate.
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
	    // TODO: get rid of the cout, but replace in your AI with code that does something
	    // useful with the information about where the opponent is shooting.
	    //cout << gotoRowCol(20, 30) << "DumbPl: opponent shot at "<< msg.getRow() << ", " << msg.getCol() << flush;
	    break;
    }
}

