/* Imports */

#include <iostream>
#include <string>
#include <utility>

// Class header
#include "text.h"

// Namespaces
using namespace std;


/************************************************
 * VNovel Text component
 * 
 * String that is written onto the background
 ************************************************/

/* Constructor */
Text::Text(std::string c, int x, int y)
{
	this->content = c;
	this->pos = make_pair(x, y);
};

/*******************************************
 * Functions
 *******************************************/

/* Playback */

// Display content on screen

/* Testing */

// Display content on command line
void Text::seeContent()
{
	cout << "@(" << this->pos.first << ", " << this->pos.second << ") " << this->content << "\n"; 
};

/* Destructor */
Text::~Text()
{
	// Nothing to be done
};