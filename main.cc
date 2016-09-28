// Brock Ferrell
// CS2401
// November 23, 2015
// Project7
#include "game.h"
#include "othello.h"
using namespace main_savitch_14;

/*! \file
*		\brief main function control for the game
*/

/** main function that declares a game, restarts, and plays.
*/
int main()
{
	Othello theGame;		/*!< main othello object */
	theGame.restart();
	theGame.play();
}
