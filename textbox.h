#ifndef TEXTBOX_H
#define TEXTBOX_H


// Base files
#include "keywords.h"
#include "vnovel.h"
#include "container.h"
#include "component.h"
#include "vnobject.h"


// Macros for error messages
// Errors happen when the object is sure that behavior is wrong
#define TEXTBOX_MAP_TYPE L"'integer'"


/* Control functions */

// Create a TextBox
unsigned int makeTextBox(VNovel* vn, unsigned int start, std::vector< std::pair<int, std::wstring> > params);

// End a TextBox, closing out all frozen and active VNObjects
unsigned int endTextBox(VNovel* vn, unsigned int start,  std::vector< std::pair<int,  std::wstring> > params);


/************************************************
 * Visual Novel TextBox
 * Component that contains and displays Text objects
 ************************************************/
class TextBox : public Component
{
	private:
		// Modifier map (keywords -> integers)
		std::map<std::wstring, int> mod;
		
		// Private default constructor
		TextBox() { }
	
	public:
		/* Constructor */
		TextBox(int loc)
		{
			//Set unicode input/output
			setUnicode(true, true);
			
			// Set identifiers
			this->type = L"TextBox";
			this->index = loc;
			
			// Default editing/traversal parameters
			this->current = 0;
			
			// Add modifiers to map
			
			// Color
			this->mod[L"r"] = 255;
			this->mod[L"g"] = 255;
			this->mod[L"b"] = 255;
			this->mod[L"alpha"] = 100;
			
			// Height
			this->mod[L"height"] = 200;
		};
		
		/*******************************************
		 * Functions
		 *******************************************/
		
		/* Build and edit */
		
		// Fill in parameters
		unsigned int setData(unsigned int start, std::vector< std::pair<int, std::wstring> > params)
		{
			for(int i = start; i < params.size(); i++)
			{
				switch(params[i].first)
				{
					// Component parameter delimiter
					case COMP_PARAM:
						if(params[i + 1].first == PARAM_VAL)
						{
							// Attempt to generate integer
							int val = toInt(params[i + 1].second);
							if(val == -1)
							{
								this->err.push_back(std::make_pair(BAD_VAL, OPEN_BRACKET + this->type + BAD_VAL_ERR + TEXTBOX_MAP_TYPE));
							}
							else
							{
								if(this->mod.count(params[i].second) == 1)
								{
									this->mod[params[i].second] = val;
								}
								else
								{
									this->err.push_back(std::make_pair(BAD_KEY, OPEN_BRACKET + this->type + BAD_KEY_ERR));
								}
							}
							
							i += 1;
						}
						else
						{
							this->err.push_back(std::make_pair(NO_VAL, OPEN_BRACKET + this->type + NO_VAL_ERR));
						}
						
						break;
					
					// Floating parameter value delimiter (skip)
					case PARAM_VAL:
						this->err.push_back(std::make_pair(NO_NAME, OPEN_BRACKET + this->type + NO_NAME_ERR));
						break;
					
					// All other delimiters
					default:
						this->err.push_back(std::make_pair(-1, DELIM_WARN_1 + this->type + DELIM_WARN_2));
						return i;
				}
			}
			
			return params.size() - 1;
		};
		
		/* Playback */
		
		// Play the Component
		// Return index to the next Component
		int play(bool gui)
		{
			int out = -1;
			
			// Play current component until output changes
			while(out == -1)
			{
				// Play and move on to the next content
				out = this->contents[this->current]->play(gui);
				this->current += 1;
				
				// Check for an ending point
				if(out > -1)
				{
					// Freeze? (Can only tell from GUI)
					
					// End?
					
				}
			}
			
			return out;
		};
		
		// Reset playback
		void resetPlay()
		{
			this->current = 0;
		};
		
		/* Destructor */
		~TextBox()
		{
			for(int i = 0; i < this->contents.size(); i++)
			{
				delete this->contents[i];
			}
		};
};


#endif