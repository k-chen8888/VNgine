/* Imports */

#include <iostream>
#include <string>
#include <vector>
#include <map>

// Class header
#include "vnovel.h"

// Parser (pushdown automata)
#include "textbox.h"

// Namespaces
using namespace std;


/* Various TextBox builder functions */

// Master function that accesses the keyword map
void* buildTextBox(Frame f, void* tb, std::vector<string> params)
{
	
};

// Set TextBox height
void* setHeight(Frame f, void* tb, std::vector<string> params)
{
	
};

// Set TextBox color
void* setColor(Frame f, void* tb, std::vector<string> params)
{
	
};

// Add a new line to TextBox (non-member function)
void* add_l(Frame f, void* tb, std::vector<string> params)
{
	int start = 0;
	
	// Create new Text object and update it
	Text temp;
	this->content.push_back(temp);
	
};

// Play contents of textbox (non-member function)
void* playTextBox(void* tb, bool cmd);


// Populate map with keywords
textbox_kw['['][L"setHeight"] = &buildTextBox; // Set textbox height
textbox_kw['['][L"setColor"] = &buildTextBox;  // Set textbox color
textbox_kw['['][L"l"] = &add_l;                // Add line of text
/*
textbox_kw['('][L"size"] = &buildTextBox;      // Set font size of text
textbox_kw['('][L"color"] = &buildTextBox;     // Set color of text
textbox_kw['('][L"space"] = &buildTextBox;     // Add a space at the end
textbox_kw['('][L"tab"] = &buildTextBox;       // Add a tab at the beginning
textbox_kw['('][L"newline"] = &buildTextBox;   // Add a newline at the end
*/


/************************************************
 * VNovel TextBox
 * 
 * Defines a textbox that represents dialog between characters
 * Textboxes span the entire screen but may have variable heights
 ************************************************/

/* Constructor */
TextBox(int h);

/*******************************************
 * Functions
 *******************************************/

/* Playback */

// Display content
void showNext();

/* Destructor */
~Text();