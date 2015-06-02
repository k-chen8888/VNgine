/* Imports */

#include <iostream>
#include <string>
#include <vector>
#include <map>

// Class header
#include "textbox.h"

// Base files
#include "vn_global.h"

// L2 Component definition
#include "component2.h"

// Namespaces
using namespace std;


/* Various TextBox builder functions */

// Master function that accesses the keyword map
Component2* buildTextBox(Frame* f, Component2* tb, std::vector<string> params)
{
	Component2* c = NULL;
	
	if(tb == NULL)
	{
		TextBox out (f->getNumComp());
		c = &out;
	}
	else
	{
		c = tb;
	}
	
	// Process modifiers, if any
	for(int i = 0; i < params.size(); i++)
	{
		if(this->setModifier(params[i], params[i + 1]) == 0) // Modifier?
		{
			i += 1;
		}
		else                                                 // A new L3 component?
		{
			// Nothing changes if it's not an L3 component
			int j = this->buildL3(i, params);
		}
	}
	
	// Add to appropriate location in Frame
	if( !(f->hasactiveComponent()) )     // No currently active component
	{
		f->addActiveComponent(c);
		if(c->hasEnd())                  // Author wrote everything on the same line
		{
			f->addComponent();
		}
		else                             // There could still be more stuff
		{
			// Nothing to do
		}
	}
	else                                 // Exists a currently active component
	{
		if( !(f->isActiveComponent(c)) ) // Check if this is the currently active component
		{
			// User forgot to close a component
			// Discard this component
		}
		else
		{
			if(c->hasEnd())              // Check if component has ended
			{
				f->addComponent();
			}
		}
	}
	
	return c;
};

// Builds the L3 components for this component (the only one that can be built is Text)
// Output location where this function stopped reading through params
unsigned int buildText(TextBox* tb, unsigned int start, std::vector<std::wstring> params)
{
	unsigned int stop = 0;
	Text t(-1, -1);
	
	// Read through params and create Text
	for(int i = start; i < params.size(); i++)
	{
		if(i = params.size() - 1)
		{
			stop = i;
		}
		
		// Add modifiers and token
		for(int i = start; i < params.size(); i++)
		{
			if(t.setModifier(params[i], params[i + 1]) == 0) // Modifier?
			{
				i += 1;
			}
			else if(tb->l3.count(params[i]) == 1)              // A new L3 component?
			{
				return i;
			}
			else                                                 // Assume it's the content
			{
				t.addContent(params[i]);
			}
		}
	}
	
	return stop;
};


/************************************************
 * VNovel TextBox
 * 
 * Defines a textbox that represents dialog between characters
 * Textboxes span the entire screen but may have variable heights
 ************************************************/

/* Constructor */
TextBox::TextBox(unsigned int loc)
{
	this->index = loc;
	this->frz = false;
	
	// Add default values to map
	
	this->mod[L"height"] = DEFAULT_HEIGHT;       // Default height of TextBox
	this->mod[L"color"] = DEFAULT_COLOR;         // TextBox background color
	this->mod[L"highlight"] = HIGHLIGHT_SPEAKER; // 1 puts a shadow over a non-speaking sprite
	
	// Add constructors for L3 components
	this->l3[L"l"] = &buildText;                 // Create/modify a TextBox
};

/*******************************************
 * Functions
 *******************************************/

/* Destructor */
TextBox::~TextBox()
{
	// Nothing to do here
};