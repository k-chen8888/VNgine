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
	bool txt = false;
	
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
		switch(params[i].first)
		{
			// L2 Parameter
			case L2_PARAM:
				// Check for parameter value delimiter immediately afterwards
				if( params[i + 1].first == PARAM_VAL )
				{
					// Attempt to add modifier
					if(c->setModifier(params[i].second, params[i + 1].second) == 0)
					{
						i += 1;
					}
				}
				else
				{
					// Discard as invalid L2_PARAM declaration
				}
				
				break;
			
			// L3 Component
			case L3_COMP:
				// Attempt to build L3 Component
				i = c->buildL3(i, params);
				break;
			
			// L3 Parameter
			case L3_PARAM:
				// Implicitly build a new L3 Text Component
				i = buildText(c, i, params);
				break;
			
			// Text to be outputted
			case TXT_TOKEN:
				// Implicitly build a new L3 Text Component
				i = buildText(c, i, params);
				break;
			
			// All other cases
			default:
				break;
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
			// Nothing to do yet
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
	Text t = new Text(-1, -1);
	
	// Read through params and create Text
	for(int i = start; i < params.size(); i++)
	{
		switch(params[i].first)
		{
			// L3 Component
			case L3_COMP:
				return i - 1;
			
			// L3 parameter
			case L3_PARAM:
			// Check for parameter value delimiter immediately afterwards
				if( params[i + 1].first == PARAM_VAL )
				{
					if(t->setModifier(params[i].second, params[i + 1].second) == 0)
					{
						i += 1;
					}
				}
				else
				{
					// Discard as invalid L3_PARAM declaration
				}
				
				break;
			
			// Text token; can no longer continue after adding this
			case TXT_TOKEN:
				t.addContent(params[i]);
				tb->addText(t);
				return i;
			
			// Discard invalid delimiters
			default:
				break;
		}
	}
	
	// Getting out here means that all parameters were used
	stop = params.size() - 1;
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
	
	// Add this component to the global keyword map
	this-addKW(L"textbox", &buildTextBox);
	
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