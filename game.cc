#include <cassert>    // Provides assert
#include <climits>    // Provides INT_MAX and INT_MIN
#include <iostream>   // Provides cin, cout
#include <queue>      // Provides queue<string>
#include <string>     // Provides string
#include "game.h"     // Provides definition of game class
using namespace std;
/*! \file
*		\brief general game template
*
*		This file acts as a general template to use for several types of
*		games to include those that utilize computer opponents
*/
namespace main_savitch_14
{
//*************************************************************************
// STATIC MEMBER CONSTANTS
// const int game::SEARCH_LEVELS;

//*************************************************************************
// PUBLIC MEMBER FUNCTIONS

/** The play function should not be overridden. It plays one round of the
* game, with the human player moving first and the computer second.
* The return value is the winner of the game (or NEUTRAL for a tie).
*/
game::who game::play( )
{
	restart( );

	while (!is_game_over( ))
	{
		display_status( );
		whosTurn();
		if (last_mover( ) == COMPUTER) {
			countingPieces();
			make_human_move( );
		}
		else {
			make_computer_move( );
		}
	}
	display_status( );
	return HUMAN;
}



//*************************************************************************
// OPTIONAL VIRTUAL FUNCTIONS (overriding these functions is optional)

/** Displays a message
*/
void game::display_message(const string& message) const
{
	cout << message;
}

/** Gets the next user move and returns it
*/
string game::get_user_move( ) const
{
	string answer;	//!< User input move

	display_message("If you cannot move, please press 'S'\n");
	display_message("Your move, please: ");
	getline(cin, answer);
	return answer;
}

/** Checks to see the current status of the game, and determines who is winning
*/
game::who game::winning()const {

	int value = evaluate(); //!< value of winner

	if (value > 0) {
		return last_mover();
	} else if (value < 0) {
		return next_mover();
	} else {
		return NEUTRAL;
	}

}

//*************************************************************************
// PRIVATE FUNCTIONS (these are the same for every game)

/*! Evaluate a board position with lookahead.
* --int look_aheads:  How deep the lookahead should go to evaluate the move.
* --int beat_this: Value of another move that we're considering. If the
* current board position can't beat this, then cut it short.
* The return value is large if the position is good for the player who just
* moved.
*/
int game::eval_with_lookahead(int look_ahead, int beat_this)
{
	queue<string> moves;   //!< All possible opponent moves
	int value;             //!< Value of a board position after opponent moves
	int best_value;        //!< Evaluation of best opponent move
	game* future;          //!< Pointer to a future version of this game

	// Base case:
	if (look_ahead == 0 || is_game_over( ))
	{
		if (last_mover( ) == COMPUTER)
			return evaluate( );
		else
			return -evaluate( );
	}

	// Recursive case:
	// The level is above 0, so try all possible opponent moves. Keep the
	// value of the best of these moves from the opponent's perspective.
	compute_moves(moves);
	// assert(!moves.empty( ));
	best_value = INT_MIN;
	while (!moves.empty( ))
	{
		future = clone( );
		future->make_move(moves.front( ));
		value = future->eval_with_lookahead(look_ahead - 1, best_value);
		delete future;
		if (value > best_value)
		{
			if (-value <= beat_this)
				return INT_MIN + 1; // Alpha-beta pruning
			best_value = value;
		}
		moves.pop( );
	}

	// The value was calculated from the opponent's perspective.
	// The answer we return should be from player's perspective, so multiply times -1:
	return -best_value;
}

/** this function makes a computer move based on
* the best possible move it computes by looking ahead and caluclating
* possible outcomes.
*/
void game::make_computer_move( )
{
	queue<string> moves; //!< queue of possible moves
	int value;					 //!< value used to determine future moves
	int best_value;			 //!< best value of the future move
	string best_move;		 //!< best move in string form
	game* future;				 //!< pointer to new future game

	// Compute all legal moves that the computer could make.
	compute_moves(moves);
	//assert(!moves.empty( ));

	// Evaluate each possible legal move, saving the index of the best
	// in best_index and saving its value in best_value.
	best_value = INT_MIN;
	while (!moves.empty( ))
	{
		future = clone( );
		future->make_move(moves.front( ));
		value = future->eval_with_lookahead(SEARCH_LEVELS, best_value);
		delete future;
		if (value >= best_value)
		{
			best_value = value;
			best_move = moves.front( );
		}
		moves.pop( );
	}

	// Make the best move.
	make_move(best_move);
}

/** This function gets the next user move, validates its legality,
* 	and makes the move
*/
void game::make_human_move( ) {
	string move;	//!< move retrieved from user input

	move = get_user_move( );
	while (!is_legal(move))
	{
		display_message("Illegal move.\n");
		move = get_user_move( );
	}
	make_move(move);
}

}
