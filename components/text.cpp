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
Text::Text(int x, int y)
{
	this->freeze = -1;
	this->end = -1;
	
	// Add default values to map
	
	// X position
	if(x > -1)
		this->mod[L"X"] = x;
	else
		this->mod[L"X"] = XPOS;
	
	// Y position
	if(y > -1)
		this->mod[L"Y"] = y;
	else
		this->mod[L"Y"] = YPOS;
	
	this->mod[L"color"] = COLOR;                 // Text color
	this->mod[L"highlight"] = HIGHLIGHT_SPEAKER; // 1 puts a shadow over a non-speaking sprite
	
	// Indices of speaker and spectator in sprite list (in Frame)
	// -1 implies no speaker/spectator
	this->mod[L"speaker"] = -1;
	this->mod[L"spectator"] = -1;
	
	// Indices of Frame background and bgm
	// -1 uses Frame defaults
	this->mod[L"framebg"] = -1;
	this->mod[L"framebgm"] = -1;
	
	// Index of Frame SFX to use
	// -1 uses no SFX
	this->mod[L"framesfx"] = -1;
	
	// Pause after outputting this line of text (0 means no, 1 means yes)
	this->mod[L"p"] = PAUSE;
};

/*******************************************
 * Functions
 *******************************************/

/* Destructor */
Text::~Text()
{
	// Nothing to be done
};